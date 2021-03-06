/****************************************************************************
 * Copyright � 2003-2012 Dorian C. Arnold, Philip C. Roth, Barton P. Miller *
 *                  Detailed MRNet usage rights in "LICENSE" file.          *
 ****************************************************************************/

// $Id: TLSKey-pthread.C,v 1.4 2007/01/24 19:34:28 darnold Exp $
#include <cassert>
#include "TLSKey-pthread.h"

namespace XPlat
{

TLSKey *XPlat_TLSKey = NULL;

TLSKey::TLSKey( void )
  : data( new PthreadTLSKeyData )
{
    // nothing else to do
}


PthreadTLSKeyData::PthreadTLSKeyData( void )
  : initialized( false )
{
    int cret = pthread_key_create( &key, NULL );
    if( cret == 0 )
    {
        initialized = true;
    }
}


PthreadTLSKeyData::~PthreadTLSKeyData( void )
{
    if( initialized )
    {
        pthread_key_delete( key );
    }
}

} // namespace XPlat

