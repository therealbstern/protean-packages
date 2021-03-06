#ifndef _PROTO_PKT
#define _PROTO_PKT

#include "protoDefs.h"  // for UINT32, etc types

#ifdef WIN32
#include "winsock2.h"   // for ntohl(), htonl(), etc
#else
#include <netinet/in.h> // for ntohl(), htonl(), etc
#endif // if/else WIN32/UNIX
#include <string.h>     // for memcpy()

// This is a base class that maintains a 32-bit aligned buffer for "packet"
// (or message) building and parsing.  Generally, classes will be derived
// from this base class to create classed for protocol-specific packet/message
// building and parsing (For examples, see ProtoPktIP, ProtoPktRTP, etc)

class ProtoPkt
{
    public:
        ProtoPkt(UINT32* bufferPtr = NULL, unsigned int numBytes = 0, bool freeOnDestruct = false);
        virtual ~ProtoPkt();
        
        bool AllocateBuffer(unsigned int numBytes)
        {
            unsigned int len = numBytes / sizeof(unsigned int);
            len += (0 == (len % sizeof(int))) ? 0 : 1;
            buffer_ptr = buffer_allocated = new UINT32[len];
            buffer_bytes = (NULL != buffer_ptr) ? numBytes : 0;
            pkt_length = 0;
            return (NULL != buffer_ptr);
        }
        void AttachBuffer(UINT32* bufferPtr, unsigned int numBytes, bool freeOnDestruct = false)
        {
            buffer_ptr = bufferPtr;
            buffer_bytes = (NULL != bufferPtr) ? numBytes : 0;
            pkt_length = 0;
            if (NULL != buffer_allocated) delete[] buffer_allocated;
            if (freeOnDestruct) buffer_allocated = bufferPtr;
        }
        UINT32* DetachBuffer()
        {
            UINT32* theBuffer = buffer_ptr;
            buffer_allocated = buffer_ptr = NULL; 
            pkt_length = buffer_bytes = 0;
            return theBuffer;  
        }
        bool InitFromBuffer(unsigned int    packetLength,
                            UINT32*         bufferPtr = NULL,
                            unsigned int    numBytes = 0,
                            bool            freeOnDestruct = false)
        {
            if (NULL != bufferPtr) AttachBuffer(bufferPtr, numBytes, freeOnDestruct);
            bool result = (packetLength <= buffer_bytes);
            pkt_length = result ? packetLength : 0;
            return result;
        }
        
        const char* GetBuffer() const {return (char*)buffer_ptr;} 
        unsigned int GetBufferLength() const {return buffer_bytes;}
        unsigned int GetLength() const {return pkt_length;} 
        
        UINT32* AccessBuffer() {return buffer_ptr;}
        void SetLength(unsigned int bytes) {pkt_length = bytes;}
            
    protected:
        
        // These helper methods are defined for setting multi-byte protocol
        // fields in one of two ways:  1) "cast and assign", or 2) memcpy()
        //
#ifdef CAST_AND_ASSIGN
        static UINT16 GetUINT16(UINT16* ptr)
            {return ntohs(*ptr);}
        static UINT32 GetUINT32(UINT32* ptr)
            {return ntohl(*ptr);}
        static void SetUINT16(UINT16* ptr, UINT16 value) 
            {*ptr = htons(value);}
        static void SetUINT32(UINT32* ptr, UINT32 value)
            {*ptr = htonl(value);}
#else
        static UINT16 GetUINT16(UINT16* ptr)
        {
            UINT16 value;
            memcpy(&value, ptr, sizeof(UINT16));
            return ntohs(value);
        } 
        static UINT32 GetUINT32(UINT32* ptr)
        {
            UINT32 value;
            memcpy(&value, ptr, sizeof(UINT32));
            return ntohl(value);
        }
        static void SetUINT16(UINT16* ptr, UINT16 value)
        {
            value = htons(value);
            memcpy(ptr, &value, sizeof(UINT16));
        }
        static void SetUINT32(UINT32* ptr, UINT32 value)
        {
            value = htonl(value);
            memcpy(ptr, &value, sizeof(UINT32));
        }
#endif // if/else CAST_AND_ASSIGN
            
            
        UINT32*         buffer_ptr;
        UINT32*         buffer_allocated;
        unsigned int    buffer_bytes;
        unsigned int    pkt_length;
        
};  // end class ProtoPkt

#endif // _PROTO_PKT
