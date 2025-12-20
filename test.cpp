#include <iostream>
#include <vector>
#include <string>
#include "MedP.h"

int main() {
	// Prepare measures for a category (e.g., Episodes, Seasons)
	std::vector<std::string> measures{"Episodes", "Seasons"};
	Category cat("Anime", "@", measures);

	// Add a media item by title into the category
	cat.add("Naruto");

	// Retrieve the media back via its title
	MedP* found = cat.getChild("Naruto");
	if (!found) {
		std::cerr << "Media not found" << std::endl;
		return 1;
	}

	// Downcast to Media to access counters/status APIs
	Media* m = dynamic_cast<Media*>(found);
	if (!m) {
		std::cerr << "Downcast to Media failed" << std::endl;
		return 1;
	}

    std::cout <<"PreTest: " << m->print() << std::endl;
	// Change status and update counts
	m->changeStatus("Ongoing");
	m->addCount();            // increment last counter
	m->addCount(0, 4);        // add 4 to first counter

	// Show different representational outputs
	std::cout << "Category print: " << cat.print() << std::endl;
	std::cout << "Category view:  " << cat.view() << std::endl;
	std::cout << "Media print:    " << m->print() << std::endl;

    //Do not delete m

	return 0;
}
