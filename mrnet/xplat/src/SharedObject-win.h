/***********************************************************************
 * Copyright � 2003-2004 Dorian C. Arnold, Philip C. Roth, Barton P. Miller *
 *                  Detailed MRNet usage rights in "LICENSE" file.     *
 **********************************************************************/

// $Id: SharedObject-win.h,v 1.2 2004/03/23 01:12:23 eli Exp $
#ifndef XPLAT_SHARED_OBJECT_UNIX_H
#define XPLAT_SHARED_OBJECT_UNIX_H

#include <winsock2.h>
#include <windows.h>
#include "xplat/SharedObject.h"

namespace XPlat
{

class WinSharedObject : public SharedObject
{
private:
    friend SharedObject* SharedObject::Load( const char* );

    HMODULE handle;

    WinSharedObject( const char* _path );

public:
    virtual ~WinSharedObject( void )
    {
        if( handle != NULL )
        {
            FreeLibrary( handle );
        }
    }

    virtual HMODULE GetHandle( void ) const   { return handle; }

    virtual void* GetSymbol( const char* sym ) const
    {
        return (void*)GetProcAddress( handle, sym );
    }
};

} // namespace XPlat

#endif // XPLAT_SHARED_OBJECT_UNIX_H