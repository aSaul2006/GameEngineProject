/************************************************************************
*  Program Name:		Vector2d.cpp									*
*  Name:				James B. Strickler II							*
*  Date:				January 25, 2013								*
*  Description:			Vector2d class used  							*
*						to create 2D vector struct						*
*  Update:																*
*																		*
************************************************************************/
#include "Vector2D.h"
#include <fstream>

std::ostream& operator<<(std::ostream& os, const Vector2D& rhs)
{
  os << " " << rhs.x << " " << rhs.y;

  return os;
}

  
std::ifstream& operator>>(std::ifstream& is, Vector2D& lhs)
{
  is >> lhs.x >> lhs.y;

  return is;
}

