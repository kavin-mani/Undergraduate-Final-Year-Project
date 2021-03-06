/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_event_finder.cpp --

  Original Author: Martin Janssen, Synopsys, Inc.
                   Stan Y. Liao, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_event_finder.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:35  acg
// SystemC 2.2
//
// Revision 1.7  2006/02/02 23:42:37  acg
//  Andy Goodrich: implemented a much better fix to the sc_event_finder
//  proliferation problem. This new version allocates only a single event
//  finder for each port for each type of event, e.g., pos(), neg(), and
//  value_change(). The event finder persists as long as the port does,
//  which is what the LRM dictates. Because only a single instance is
//  allocated for each event type per port there is not a potential
//  explosion of storage as was true in the 2.0.1/2.1 versions.
//
// Revision 1.6  2006/02/02 21:26:34  acg
//  Andy Goodrich: pulled out the check I just stuck into the
//  sc_event_finder::free_instances() method. It turns out the LRM says that
//  sc_event_finder instances are valid as long as the sc_module hierarchy is
//  valid, so we can't give the user a call to free the instances.
//
// Revision 1.5  2006/02/02 21:10:52  acg
//  Andy Goodrich: added check for end of elaboration to the static method
//  sc_event_finder::free_instances(). This will allow the method to be
//  made public if that is desired.
//
// Revision 1.4  2006/02/02 20:43:09  acg
//  Andy Goodrich: Added an existence linked list to sc_event_finder so that
//  the dynamically allocated instances can be freed after port binding
//  completes. This replaces the individual deletions in ~sc_bind_ef, as these
//  caused an exception if an sc_event_finder instance was used more than
//  once, due to a double freeing of the instance.
//
// Revision 1.3  2006/01/13 18:47:41  acg
// Added $Log command so that CVS comments are reproduced in the source.
//

#include "sysc/communication/sc_event_finder.h"

namespace sc_core
{

// ----------------------------------------------------------------------------
//  CLASS : sc_event_finder
//
//  Event finder base class.
// ----------------------------------------------------------------------------

// error reporting

void
sc_event_finder::report_error( const char* id, const char* add_msg ) const
{
    char msg[BUFSIZ];
    if ( add_msg != 0 )
    {
        std::sprintf( msg, "%s: port '%s' (%s)",
                      add_msg, m_port.name(), m_port.kind() );
    }
    else
    {
        std::sprintf( msg, "port '%s' (%s)", m_port.name(), m_port.kind() );
    }
    SC_REPORT_ERROR( id, msg );
}


// constructor

sc_event_finder::sc_event_finder( const sc_port_base& port_ )
        : m_port( port_ )
{
}


// destructor (does nothing)

sc_event_finder::~sc_event_finder()
{}

} // namespace sc_core

// Taf!
