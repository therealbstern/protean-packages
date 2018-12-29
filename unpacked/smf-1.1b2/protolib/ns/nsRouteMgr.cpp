#include "protoRouteMgr.h"
//#include "protoRouteTable.h"
//#include "protoSim.h"

class NsRouteMgr : public ProtoRouteMgr
{
    public:
          NsRouteMgr();
          ~NsRouteMgr();
          virtual bool Open(const void* userData = NULL);
          virtual bool IsOpen() const {return local_table.IsReady();}
          virtual void Close();

          virtual bool GetAllRoutes(ProtoAddress::Type addrType,
			            ProtoRouteTable& routeTable);

          virtual bool GetRoute(const ProtoAddress& dst,
			        unsigned int        prefixLen,
			        ProtoAddress&       gw,
			        unsigned int&       ifIndex,
			        int&                metric);

          virtual bool SetRoute(const ProtoAddress& dst,
			        unsigned int          prefixLen,
			        const ProtoAddress& gw,
			        unsigned int          ifIndex = 0,
			        int                   metric = -1);

          virtual bool DeleteRoute(const ProtoAddress& dst,
			           unsigned int          prefixLen,
			           const ProtoAddress& gw,
			           unsigned int          ifIndex = 0);  
          virtual bool SetForwarding(bool state);

          bool GetInterfaceAddressList(unsigned int        ifIndex, 
			                           ProtoAddress::Type  addrType,
			                           ProtoAddress::List& theAddress);   
          unsigned int GetInterfaceIndex(const char* interfaceName);
          bool GetInterfaceName(unsigned int  interfaceIndex, 
			        char*         buffer, 
			        unsigned int  buflen);
  
      private:
          int               node_id;
          int               forwarding_on;
          ProtoRouteTable   local_table;
}; //end class NsRouteMgr

ProtoRouteMgr* ProtoRouteMgr::Create()
{
  return (ProtoRouteMgr*)(new NsRouteMgr);
} // end ProtoRouteMgr::Create()

NsRouteMgr::NsRouteMgr()
{
}

NsRouteMgr::~NsRouteMgr()
{
  Close();
}

bool NsRouteMgr::Open(const void* userData)
{
    node_id = *(int*)userData;
    return local_table.Init();
  //empty? maybe need ProtoRouteTable here?
}

void NsRouteMgr::Close()
{
    local_table.Destroy();
  //empty? maybe need ProtoRouteTable delete here?
}

bool NsRouteMgr::GetRoute(const ProtoAddress& dst,
			  unsigned int        prefixLen,
			  ProtoAddress&       gw,
			  unsigned int&       ifIndex,
			  int&                metric)
{
  return local_table.FindRoute(dst,prefixLen,gw,ifIndex,metric);
}

bool NsRouteMgr::SetRoute(const ProtoAddress& dst,
			  unsigned int          prefixLen,
			  const ProtoAddress& gw,
			  unsigned int          ifIndex,
			  int                   metric)
{
  return local_table.SetRoute(dst,prefixLen,gw,ifIndex,metric);
  //call ProtoRouteTable setroute if there?
} // end NsRouteMgr::SetRoute()

bool NsRouteMgr::DeleteRoute(const ProtoAddress& dst,
                                unsigned int          prefixLen,
                                const ProtoAddress& gw,
                                unsigned int          ifIndex)
{
  return local_table.DeleteRoute(dst,prefixLen,&gw);
} // end NsRouteMgr::DeleteRoute()

bool NsRouteMgr::GetAllRoutes(ProtoAddress::Type addrType,
				 ProtoRouteTable&   routeTable)
{
    return false; //this function does not currently work in ns
}// end NsRouteMgr::GetAllRoutes

bool NsRouteMgr::GetInterfaceAddressList(unsigned int        ifIndex, 
                                         ProtoAddress::Type  addrType,
                                         ProtoAddress::List& addrList)
{
    //this is going to be a little trickier
    ProtoAddress ifAddr;
    ifAddr.SimSetAddress(node_id);
    return (addrList.Insert(ifAddr));
}  // end NsRouteMgr::GetInterfaceAddress()

unsigned int NsRouteMgr::GetInterfaceIndex(const char* interfaceName)
{
    return 1;
}  // end NsRouteMgr::GetInterfaceIndex()

bool NsRouteMgr::GetInterfaceName(unsigned int interfaceIndex,
		              char*        buffer,
		              unsigned int buflen)
{
  snprintf(buffer,buflen,"%d",node_id);
  return true;
}  // end NsRouteMgr::GetInterfaceName()

bool NsRouteMgr::SetForwarding(bool state)
{
  forwarding_on = state;
  return true;
}  // end NsRouteMgr::SetForwarding()
