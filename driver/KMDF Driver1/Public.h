/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that app can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_KMDFDriver1,
    0xba71f3a6,0x29fb,0x42ff,0xa1,0x63,0x84,0x76,0x25,0x9f,0xc3,0x62);
// {ba71f3a6-29fb-42ff-a163-8476259fc362}
