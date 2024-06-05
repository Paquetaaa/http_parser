/* Taken from FCGI specification and adapted */ 

#define FASTCGILENGTH 0xffff

#define FASTCGIMAXNVPAIR 0x7fff

/*
 * Value for version component of FCGI_Header
 */
#define FCGI_VERSION_1           1

/* Represents the header of a fastcgi request (web -> app) */
typedef struct __attribute__((__packed__)) {
    unsigned char version;           // FastCGI protocol version
    unsigned char type;              // FastCGI record type : general function performing here
    unsigned short requestId;        // Which request this record belongs
    unsigned short contentLength;    // Number of bytes in the contentData component 
    unsigned char paddingLength;     // Number of bytes in the paddingData component (paddingData contains 0 - 255 Bytes that are ignored)
    unsigned char reserved;          // Seems to contain everything above the transport layer ??
    char contentData[FASTCGILENGTH]; // Between 0 & 65353 bytes of data, treated as described with the type
} FCGI_Header;  // Records must be placed on boundaries of 8-bytes mutiples)

#define FCGI_HEADER_SIZE           8

/*
 * Value for requestId component of FCGI_Header
 */
#define FCGI_NULL_REQUEST_ID     0

/*
 * Values for type component of FCGI_Header
 */
#define FCGI_BEGIN_REQUEST       1
#define FCGI_ABORT_REQUEST       2
#define FCGI_END_REQUEST         3
#define FCGI_PARAMS              4
#define FCGI_STDIN               5
#define FCGI_STDOUT              6
#define FCGI_STDERR              7
#define FCGI_DATA                8
#define FCGI_GET_VALUES          9
#define FCGI_GET_VALUES_RESULT  10
#define FCGI_UNKNOWN_TYPE       11
#define FCGI_MAXTYPE (FCGI_UNKNOWN_TYPE)



typedef struct __attribute__((__packed__)) {
            unsigned char type;         // The unknown type received
            unsigned char reserved[7];
} FCGI_UnknownTypeBody; // Response to a FCGI_HEADER where the type is unknown of the app


typedef struct __attribute__((__packed__)) {
    unsigned short role;        // 3 roles : FCGI_RESPONDER / FCGI_AUTHORIZE / FCGI_FILTER (cf notes page 1)
    unsigned char flags;        // Flag about the connection control (always false | 0, close the conn after the response send)
    unsigned char unused[5];
}  FCGI_BeginRequestBody;


typedef struct __attribute__((__packed__)) {
            unsigned int appStatus;         // Responder role -> what returned the exit system call | Authorizer && Filter are similar 
            unsigned char protocolStatus;   // Can be : FCGI_REQUEST_COMPLETE | FCGI_CANT_MPX_CONN (rejecting new request <-> concurrent conn) 
                                            // | FCGI_OVERLOADED (rejecting new request <-> short of ressoruces) | FCGI_UNKNOWN_ROLE (rejecting new ressources <-> role unknown by the app)
            unsigned char reserved[3];
} FCGI_EndRequestBody;




/*
 * Values for protocolStatus component of FCGI_EndRequestBody
 */
#define FCGI_REQUEST_COMPLETE 0
#define FCGI_CANT_MPX_CONN    1
#define FCGI_OVERLOADED       2
#define FCGI_UNKNOWN_ROLE     3

/*
 * Variable names for FCGI_GET_VALUES / FCGI_GET_VALUES_RESULT records
 */
#define FCGI_MAX_CONNS  "FCGI_MAX_CONNS"
#define FCGI_MAX_REQS   "FCGI_MAX_REQS"
#define FCGI_MPXS_CONNS "FCGI_MPXS_CONNS"

/*
 * Mask for flags component of FCGI_BeginRequestBody
 */
#define FCGI_KEEP_CONN  1

#define FCGI_RESPONDER  1
#define FCGI_AUTHORIZER 2
#define FCGI_FILTER     3

