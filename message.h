#ifndef MESSAGE_H
#define MESSAGE_H

#include<iostream>
#include<semaphore.h>
using namespace std;

template <class T>
class Message
{
    private:
        // the index to read from
        bool read_index;
        // semaphores to use by reading thread when reading from a value
        sem_t read_sems[2];
        // indicate if new data has been written to a value
        bool dirty[2];
        // values to read and write from, acts as a double buffer
        T values[2];

    public:
        Message()   // default constructor
        {
            read_index = 0;
            dirty = {0,0};

            // initialize binary semaphores
            sem_init(&read_sems[0],0,1);
            sem_init(&read_sems[1],0,1);
        }
        ~Message()  // destructor
        {}

        //-----------------------------------------------------------
        // Function: getMessage
        //  Purpose: To be used by a reader thread to get data from the
        //              buffer. Handles retrieving data from the correct
        //              buffer and returns false if there is no change
        //              to the data since the last read.
        //   Inputs: T& to place the read data into
        //  Outputs: bool for if the data has changed since the last read
        //-----------------------------------------------------------
        bool getMessage(T &value)
        {
            // procure read semaphore
            sem_wait(&read_sems[read_index]);
            // if not dirty, return false
            if(!dirty[read_index])
                return false;
            // else, if it is dirty
            // place value into the passed-in value
            value = values[read_index];
            // reset dirty flag
            dirty[read_index] = 0;
            // update read index
            read_index = !read_index;
            // vacate the semaphore that was procured at the start
            sem_post(&read_sems[!read_index]);
            // return true
            return true;
        }

        //-----------------------------------------------------------
        // Function: putMessage
        //  Purpose: To be used by a writer thread to place data into
        //              a buffer. Handles writing data to the correct
        //              buffer and indicating if the data has changed
        //              since the last write.
        //   Inputs: T to copy into the correct value,
        //           bool to say if the value has changed
        //  Outputs: void
        //-----------------------------------------------------------
        void putMessage(T value, bool incoming_dirty)
        {
            // procure semaphore on the buffer that shouldn't be being accessed by the reader
            sem_wait(&read_sems[!read_index]);
            // if not dirty, return
            if(!incoming_dirty)
                return;
            // if it is dirty
            // place value that was passed in into the message values
            values[!read_index] = value;
            // set the dirty flag
            dirty[!read_index] = 1;
            // vacate the semaphore that was procured at the start
            sem_post(&read_sems[!read_index]);
        }

};

#endif // MESSAGE_H
