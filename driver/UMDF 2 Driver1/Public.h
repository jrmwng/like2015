/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    driver and application

--*/

//
// Define an Interface Guid so that app can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_UMDF2Driver1,
    0x42cc6810,0x7991,0x4d4c,0x8d,0xa6,0x9a,0x44,0xd8,0x65,0x07,0x1b);
// {42cc6810-7991-4d4c-8da6-9a44d865071b}
