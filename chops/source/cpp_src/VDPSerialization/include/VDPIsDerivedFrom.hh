#ifndef VDPIsDerivedFrom_HH__
#define VDPIsDerivedFrom_HH__



/**
*
*@file VDPIsDerivedFrom.hh
*@class VDPIsDerivedFrom
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Mon Feb  4 13:46:14 EST 2013 J. Barrett (barrettj@mit.edu) First Version
*
*/

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//This is some crazy Alexandrescu stuff I won't pretend to understand that helps
//us make a template specialization when a particular template parameter is
//derived from a particular type

//When D is derived from B, then VDPIsDerivedFrom::Is = 1,
//When D is not derived from B, the VDPIsDerivedFrom::Is = 0

//taken from: http://www.gotw.ca/publications/mxc++-item-4.htm


template<typename D, typename B>
class VDPIsDerivedFrom
{
  class No { };
  class Yes { No no[3]; };

  static Yes Test( B* ); // not defined
  static No Test( ... ); // not defined

  static void Constraints(D* p) { B* pb = p; pb = p; }

public:
  enum { Is = sizeof(Test(static_cast<D*>(0))) == sizeof(Yes) };

  VDPIsDerivedFrom() { void(*p)(D*) = Constraints; }
};




#endif /* VDPIsDerivedFrom_H__ */
