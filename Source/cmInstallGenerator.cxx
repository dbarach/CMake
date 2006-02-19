/*=========================================================================

  Program:   CMake - Cross-Platform Makefile Generator
  Module:    $RCSfile$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cmInstallGenerator.h"

#include "cmSystemTools.h"
#include "cmTarget.h"

//----------------------------------------------------------------------------
cmInstallGenerator
::cmInstallGenerator()
{
  this->ConfigurationName = 0;
  this->ConfigurationTypes = 0;
}

//----------------------------------------------------------------------------
cmInstallGenerator
::~cmInstallGenerator()
{
}

//----------------------------------------------------------------------------
void
cmInstallGenerator
::Generate(std::ostream& os, const char* config,
           std::vector<std::string> const& configurationTypes)
{
  this->ConfigurationName = config;
  this->ConfigurationTypes = &configurationTypes;
  this->GenerateScript(os);
  this->ConfigurationName = 0;
  this->ConfigurationTypes = 0;
}

//----------------------------------------------------------------------------
void cmInstallGenerator::AddInstallRule(std::ostream& os,
                                        const char* dest,
                                        int type,
                                        const char* files,
                                        bool optional /* = false */,
                                        const char* properties /* = 0 */)
{
  // TODO: Make optional files use IF(EXISTS) to not report if not
  // installing.
  std::string sfiles = files;
  std::string destination = dest;
  std::string stype;
  switch(type)
    {
    case cmTarget::INSTALL_PROGRAMS: stype = "PROGRAM"; break;
    case cmTarget::EXECUTABLE:       stype = "EXECUTABLE"; break;
    case cmTarget::STATIC_LIBRARY:   stype = "STATIC_LIBRARY"; break;
    case cmTarget::SHARED_LIBRARY:   stype = "SHARED_LIBRARY"; break;
    case cmTarget::MODULE_LIBRARY:   stype = "MODULE"; break;
    case cmTarget::INSTALL_FILES:
    default:                         stype = "FILE"; break;
    }
  std::string fname = cmSystemTools::GetFilenameName(sfiles.c_str());
  os << "MESSAGE(STATUS \"Installing " << destination.c_str()
     << "/" << fname.c_str() << "\")\n"
     << "FILE(INSTALL DESTINATION \"" << destination.c_str()
     << "\" TYPE " << stype.c_str() << (optional?" OPTIONAL":"") ;
  if(properties && *properties)
    {
    os << " PROPERTIES" << properties;
    }
  os << " FILES \"" << sfiles.c_str() << "\")\n";
}
