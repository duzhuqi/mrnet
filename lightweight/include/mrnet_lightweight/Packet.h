/****************************************************************************
 *  Copyright 2003-2010 Dorian C. Arnold, Philip C. Roth, Barton P. Miller  *
 *                  Detailed MRNet usage rights in "LICENSE" file.          *
 ****************************************************************************/


#if !defined(__packet_h)
#define __packet_h 1

#include <stdarg.h>

#include "mrnet_lightweight/DataElement.h"
#include "mrnet_lightweight/Types.h"

struct vector_t;
struct PDR;

typedef struct {
  uint16_t stream_id;
  int32_t tag; // application/protocol level ID
  Rank src_rank;  // null terminated string
  char* fmt_str;  // null terminated string
  char* buf;  // the entire packed buffer (header+payload)
  unsigned int buf_len;
  Rank inlet_rank;
  int destroy_data;
  struct vector_t* data_elements;
} Packet_t;


Packet_t* new_Packet_t(int isrc, unsigned short istream_id, int itag, 
                       char* fmt, va_list arg_list);

Packet_t* new_Packet_t_2(unsigned short istream_id, int itag, char* fmt, ...);

Packet_t* new_Packet_t_3(unsigned int buf_len, char* buf, Rank inlet_rank);

Packet_t* Packet_construct(Packet_t* packet);

void Packet_set_DestroyData(Packet_t * packet, int dd);

int Packet_get_Tag(Packet_t* packet);

unsigned short Packet_get_StreamId(Packet_t* packet);

char* Packet_get_FormatString(Packet_t* packet);

void Packet_ArgList2DataElementArray(Packet_t* packet, va_list arg_list);

Packet_t* Packet_pushBackElement(Packet_t* packet, DataElement_t* cur_elem);

bool_t Packet_pdr_packet(struct PDR *pdrs,  Packet_t* pkt);

int Packet_unpack(Packet_t* packet, const char *ifmt_str, ...);

int Packet_ExtractVaList(Packet_t* packet, char* fmt, va_list arg_list);

void Packet_DataElementArray2ArgList(Packet_t* packet, va_list arg_list);

int Packet_ExtractArgList(Packet_t* packet, char* ifmt_str, ...);

#endif /* __packet_h */
