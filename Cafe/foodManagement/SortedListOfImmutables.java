package foodManagement;

/**
 * A SortedListOfImmutables represents a sorted collection of immutable objects
 * that implement the Listable interface.
 * 
 * An array of references to Listable objects is used internally to represent
 * the list.
 * 
 * The items in the list are always kept in alphabetical order based on the
 * names of the items. When a new item is added into the list, it is inserted
 * into the correct position so that the list stays ordered alphabetically by
 * name.
 * 
 * It has the ability to get the index of an item in the list, the size of the
 * list, add one or more items to the list, remove one or more items to the
 * list, get the Wholesale value of the list, the retail value of the list,
 * checks the availability of an item or multiple in the list
 */
public class SortedListOfImmutables {

	private Listable[] items;

	/**
	 * This constructor creates an empty list by creating an internal array of
	 * size 0.
	 */
	public SortedListOfImmutables() {
		this.items = new Listable[0];
	}

	/**
	 * Copy constructor. The current object will become a copy of the list that
	 * the parameter refers to. A shallow copy.
	 */
	public SortedListOfImmutables(SortedListOfImmutables other) {
		Listable[] copy = new Listable[other.items.length];
		for (int i = 0; i < other.items.length; i++) {
			copy[i] = other.items[i];
		}

		items = copy;
	}

	// Returns the number of items in the list.
	public int getSize() {
		return items.length;
	}

	/**
	 * Returns a reference to the item in the ith position in the list.
	 * (Indexing is 0-based, so the first element is element 0).
	 * 
	 * @param i index of item requested
	 * @return reference to the ith item in the list
	 */
	public Listable get(int i) {
		return items[i];
	}

	/**
	 * Adds an item to the list. This method assumes that the list is already
	 * sorted in alphabetical order based on the names of the items in the list.
	 * 
	 * The new item will be inserted into the list in the appropriate place so
	 * that the list will remain alphabetized by names.
	 * 
	 * 
	 * @param itemToAdd refers to a Listable item to be added to this list
	 */
	public void add(Listable itemToAdd) {
		/**
		 * In order to accommodate the new item, the internal array must be
		 * re-sized so that it is one unit larger than it was before the call to
		 * this method.
		 */
		Listable[] copy = new Listable[items.length + 1];
		// adds the item to the end
		copy[copy.length - 1] = itemToAdd;

		for (int i = 0; i < items.length; i++) {
			// copies the list onto the copy except the last index of the copy
			copy[i] = items[i];

			// if alphabetically the item is next, its added to it's correct
			// spot
			if (itemToAdd.getName().compareTo(items[i].getName()) < 0) {
				copy[i] = itemToAdd;

				// the rest of the list is copied one item back
				for (int j = i + 1; j < copy.length; j++) {
					copy[j] = items[j - 1];
				}
				break;
			}
		}
		items = copy;
	}

	/**
	 * Adds an entire list of items to the current list, maintaining the
	 * alphabetical ordering of the list by the names of the items.
	 * 
	 * @param listToAdd a list of items that are to be added to the current
	 *                  object
	 */
	public void add(SortedListOfImmutables listToAdd) {

		// Goes through the list and adds all of the items in the listToAdd
		for (int i = 0; i < listToAdd.items.length; i++) {
			this.add(listToAdd.items[i]);
		}

	}

	/**
	 * Removes an item from the list. If the list contains the same item that
	 * the parameter refers to, it will be removed from the list. If the item
	 * appears in the list more than once, just one instance will be removed. If
	 * the item does not appear on the list, then this method does nothing.
	 * 
	 * @param itemToRemove refers to the item that is to be removed from the
	 *                     list
	 */
	public void remove(Listable itemToRemove) {

		// Checks if the item is on the menu
		if (checkAvailability(itemToRemove)) {
			int index = 0;

			// Creates an index for which position the itemToRemove is in
			for (int i = 0; i < items.length; i++) {
				if (items[i] == itemToRemove) {
					index = i;
					break;
				}
			}

			// Create a copy of the list and removes an instance
			Listable[] copy = new Listable[items.length - 1];
			// Copies the entire list
			for (int i = 0; i < index; i++) {
				copy[i] = items[i];
			}
			// Removing the itemToRemove and shifting the list forwards by one
			for (int i = index + 1; i < items.length; i++) {
				copy[i - 1] = items[i];

			}

			items = copy;

		} else {
			// If the item is not on the menu then nothing happens
		}

	}

	/**
	 * Removes an entire list of items from the current list. Any items in the
	 * parameter that appear in the current list are removed; any items in the
	 * parameter that do not appear in the current list are ignored.
	 * 
	 * @param listToRemove list of items that are to be removed from this list
	 */
	public void remove(SortedListOfImmutables listToRemove) {
		// Goes through the list and removes all of the items in the
		// listToRemove
		for (int i = 0; i < listToRemove.items.length; i++) {
			this.remove(listToRemove.items[i]);
		}
	}

	/**
	 * Returns the sum of the wholesale costs of all items in the list.
	 * 
	 * @return sum of the wholesale costs of all items in the list
	 */
	public int getWholesaleCost() {
		int totalValue = 0;

		// Goes through the list and adds the wholesale cost to the total value
		for (int i = 0; i < items.length; i++) {
			totalValue += items[i].getWholesaleCost();
		}

		return totalValue;
	}

	/**
	 * Returns the sum of the retail values of all items in the list.
	 * 
	 * @return sum of the retail values of all items in the list
	 */
	public int getRetailValue() {
		int totalValue = 0;

		// Goes through the list and adds the retail cost to the total value
		for (int i = 0; i < items.length; i++) {
			totalValue += items[i].getRetailValue();
		}

		return totalValue;
	}

	/**
	 * Checks to see if a particular item is in the list.
	 * 
	 * @param itemToFind item to look for
	 * @return true if the item is found in the list, false otherwise
	 */
	public boolean checkAvailability(Listable itemToFind) {

		// Goes through the list to check if the item is in the list
		for (int i = 0; i < items.length; i++) {
			// if the item is in the list it returns true
			if (items[i].equals(itemToFind)) {
				return true;
			}
		}
		return false;
	}

	/**
	 * Checks if a list of items is contained in the current list. (In other
	 * words, this method will return true if ALL of the items in the parameter
	 * are contained in the current list. If anything is missing, then the
	 * return value will be false.)
	 * 
	 * @param listToCheck list of items that may or may not be a subset of the
	 *                    current list
	 * @return true if the parameter is a subset of the current list; false
	 *         otherwise
	 */
	public boolean checkAvailability(SortedListOfImmutables listToCheck) {
		SortedListOfImmutables copy = new SortedListOfImmutables(this);

		for (int i = 0; i < listToCheck.getSize(); i++) {

			/* checks to see if all of the items on the list is available if
			   not, return true */
			if ((checkAvailability(listToCheck.items[i]) == false)) {
				return false;

			} else {
				// If the item exists in the list, it is removed
				for (int j = 0; j < copy.getSize(); j++) {
					if (copy.items[j].equals(listToCheck.items[i])) {
						copy.remove(copy.items[j]);
						break;
					} else if (j == copy.getSize() - 1) {
						return false;
					}
					
				}
				
			}
		}
		// If it went through the entire listToCheck it will return true
		return true;
	}

	// Creates a string of the items in the list
	public String toString() {
		String retValue = "[ ";
		for (int i = 0; i < items.length; i++) {
			if (i != 0) {
				retValue += ", ";
			}
			retValue += items[i];
		}
		retValue += " ]";
		return retValue;
	}
}
