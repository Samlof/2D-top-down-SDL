#pragma once

#include <memory>
#include <unordered_map>

class InventoryItem;

class ItemManager {
public:
	ItemManager();
	~ItemManager();

	// Assigns to ItemMap. Delete thru itemmap or item.erase
	InventoryItem* createPickableItem(int pId, const int pAmount);
	// Doesn't assign to ItemMap. Deletion is on caller to handle
	static InventoryItem* createLocalPickableItem(int pId, const int pAmount);
	void deleteItem(InventoryItem* pItem);
private:

	using ItemMap = std::unordered_multimap<int, std::unique_ptr<InventoryItem> >;
	ItemMap mItems;
};