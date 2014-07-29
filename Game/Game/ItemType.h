#ifndef _ITEMTYPE
#define _ITEMTYPE
#include <ostream>
enum ItemType {
	Sword,
	Helmet,
	Shield,
	Booster
};

std::ostream& operator<<(std::ostream& os, const ItemType& it);

#endif
