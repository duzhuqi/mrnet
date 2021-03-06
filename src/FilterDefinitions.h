/****************************************************************************
 *  Copyright 2003-2015 Dorian C. Arnold, Philip C. Roth, Barton P. Miller  *
 *                  Detailed MRNet usage rights in "LICENSE" file.          *
 ****************************************************************************/

#if !defined( filterdefinitions_h )
#define filterdefinitions_h 1

#include <vector>
#include <set>

#include "mrnet/FilterIds.h"
#include "mrnet/NetworkTopology.h"
#include "mrnet/Packet.h"

namespace MRN
{

class PeerNode;

// transformation filters
extern const char * TFILTER_NULL_FORMATSTR;

extern const char * TFILTER_SUM_FORMATSTR;
void tfilter_Sum( const std::vector < PacketPtr >&, 
                  std::vector < PacketPtr >&, 
                  std::vector < PacketPtr >&, 
                  void**, PacketPtr&, const TopologyLocalInfo& );

extern const char * TFILTER_AVG_FORMATSTR;
void tfilter_Avg( const std::vector < PacketPtr >&, 
                  std::vector < PacketPtr >&, 
                  std::vector < PacketPtr >&, 
                  void**, PacketPtr&, const TopologyLocalInfo& );

extern const char * TFILTER_MIN_FORMATSTR;
void tfilter_Min( const std::vector < PacketPtr >&, 
                  std::vector < PacketPtr >&, 
                  std::vector < PacketPtr >&, 
                  void**, PacketPtr&, const TopologyLocalInfo& );

extern const char * TFILTER_MAX_FORMATSTR;
void tfilter_Max( const std::vector < PacketPtr >&, 
                  std::vector < PacketPtr >&, 
                  std::vector < PacketPtr >&, 
                  void**, PacketPtr&, const TopologyLocalInfo& );

extern const char * TFILTER_ARRAY_CONCAT_FORMATSTR;
void tfilter_ArrayConcat( const std::vector < PacketPtr >&, 
                          std::vector < PacketPtr >&, 
                          std::vector < PacketPtr >&, 
                          void**, PacketPtr&, const TopologyLocalInfo& );

extern const char * TFILTER_INT_EQ_CLASS_FORMATSTR;
void tfilter_IntEqClass( const std::vector < PacketPtr >&, 
                         std::vector < PacketPtr >&, 
                         std::vector < PacketPtr >&, 
                         void**, PacketPtr&, const TopologyLocalInfo& );

extern const char * TFILTER_PERFDATA_FORMATSTR;
void tfilter_PerfData( const std::vector < PacketPtr >&, 
                       std::vector < PacketPtr >&, 
                       std::vector < PacketPtr >&, 
                       void**, PacketPtr&, const TopologyLocalInfo& );

extern const char* TFILTER_TOPO_UPDATE_FORMATSTR;
void tfilter_TopoUpdate(const std::vector < PacketPtr >&,
                       std::vector < PacketPtr>&,
		       std::vector < PacketPtr>&,
		       void**, PacketPtr&, const TopologyLocalInfo& );

extern const char* TFILTER_TOPO_UPDATE_DOWNSTREAM_FORMATSTR;
void tfilter_TopoUpdate_Downstream(const std::vector < PacketPtr >&,
                       std::vector < PacketPtr>&,
	               std::vector < PacketPtr>&,
	               void**, PacketPtr&, const TopologyLocalInfo& );

// synchronization filters
void sfilter_WaitForAll( const std::vector < PacketPtr >&, 
                         std::vector < PacketPtr >&, 
                         std::vector < PacketPtr >&, 
                         void**, PacketPtr&, const TopologyLocalInfo& );

void sfilter_TimeOut( const std::vector < PacketPtr >&, 
                      std::vector < PacketPtr >&, 
                      std::vector < PacketPtr >&, 
                      void**, PacketPtr&, const TopologyLocalInfo& );

} // namespace MRN

#endif  /* filterdefinitions_h */
