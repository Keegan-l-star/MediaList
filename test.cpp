#include <iostream>
#include <vector>
#include <string>
#include "MedP.h"
#include "Repo.h"

void test_assert(bool condition, const std::string& message) {
	if (!condition) {
		std::cerr << "Test failed: " << message << std::endl;
	}else{
		std::cout << "Test passed: " <<std::endl;
	}
}

void contentTest(){
	// Prepare measures for a category (e.g., Episodes, Seasons)
	std::vector<std::string> measures{"Episodes", "Seasons"};
	Category cat("Anime", "@", measures);

	// Add a media item by title into the category
	cat.add("Naruto");

	// Retrieve the media back via its title
	MedP* found = cat.getChild("Naruto");
	test_assert(found != nullptr, "Media not found in category");

	// Downcast to Media to access counters/status APIs
	Media* m = dynamic_cast<Media*>(found);
	test_assert(m != nullptr, "Downcasting to Media");

    std::cout <<"PreTest: " << m->print() << std::endl;
	// Change status and update counts
	m->changeStatus("Ongoing");
	m->addCount();            // increment last counter
	m->addCount(0, 4);        // add 4 to first counter

	Category* foundCat = cat.cat("Anime");
	test_assert(foundCat != nullptr, "(Anime) Category not found");
	test_assert((foundCat->cat("Movie")==nullptr), "Retrieved Category does not match Anime");

	// Show different representational outputs
	std::string testPrintCat="![<@>Anime|{Episodes,Seasons}]\n";
	//std::cout<<cat.print()<<std::endl;
	test_assert(cat.print() == testPrintCat, "Category print representation mismatch");
	std::string testViewCat="<@>(Naruto){5,2}\\\\Ongoing\n";
	test_assert(cat.view() == testViewCat, "Category view representation mismatch");
	std::string testPrintMed="(Naruto){5,2}\\\\Ongoing";
	test_assert(m->print() == testPrintMed, "Media print representation mismatch");
    //Do not delete m
}

void RepoTest(){
	Repo repo;

	// Add a theme/category
	Category* cat = repo.addTheme("Anime", "@", {"Episodes", "Seasons"});
	repo.addTheme("Movie", "#", {"Duration"});
	test_assert(cat != nullptr, "Failed to add category");
	// Add media to the category
	Media* media = repo.addMedia("Anime", "Naruto");
	repo.addMedia("Movie", "Inception");
	std::vector<std::string> counters{"chapter","page"};
	Media* media2 = new Media(counters, "That time I got reincarnated as a slime");
	test_assert(media != nullptr, "Failed to add media to category");
	repo.addTheme("Manga", "@", counters);//Duplicate symbol test
	test_assert(repo.getTheme("Manga") == nullptr, "Added duplicate symbol category");
	repo.addTheme("Manga","+",counters);
	repo.addMedia("Manga", media2);
	test_assert(repo.getMedia("Manga", "That time I got reincarnated as a slime") == media2, "Failed to add media object to Manga category");

	// Retrieve media
	Media* retrieved = repo.getMedia("Anime", "Naruto");
	test_assert(retrieved != nullptr, "Failed to retrieve media from category");

	// Change media status and counts
	repo.status("Anime", "Naruto", "Ongoing");
	repo.changeMedia("Anime", "Naruto", 0, 4); // Add 4 to first counter
	retrieved->addCount(); // Increment last counter

	// Validate outputs
	std::cout<<"Repo Test Print: \n"<< repo.print() << std::endl;
}

int main() {
	contentTest();
	RepoTest();
	return 0;
}
