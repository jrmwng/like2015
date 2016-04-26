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
    0xe4524a5c,0x2682,0x4fd9,0x80,0x8e,0xa6,0xb7,0x15,0xf2,0x22,0x46);
// {e4524a5c-2682-4fd9-808e-a6b715f22246}
