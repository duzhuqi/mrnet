/****************************************************************************
 * Copyright � 2003-2012 Dorian C. Arnold, Philip C. Roth, Barton P. Miller *
 *                  Detailed MRNet usage rights in "LICENSE" file.          *
 ****************************************************************************/

// $Id: PathUtils-unix.C,v 1.3 2007/01/24 19:34:17 darnold Exp $
#include <cstring>
#include <libgen.h>
#include "xplat/PathUtils.h"

namespace XPlat
{

namespace PathUtils
{

std::string
GetFilename( const std::string& path )
{
    // basename modifies the path
    char* pathCopy = new char[path.length() + 1];
    strcpy( pathCopy, path.c_str() );

    std::string ret = basename( pathCopy );

    delete[] pathCopy;

    return ret;
}

std::string
GetDirname( const std::string& path )
{
    // dirname modifies the path
    char* pathCopy = new char[path.length() + 1];
    strcpy( pathCopy, path.c_str() );

    std::string ret = dirname( pathCopy );

    delete[] pathCopy;

    return ret;
}

} // namespace PathUtils

} // namespace XPlat

