#include <iostream>
#include <vector>
#include <string>
#include "MedP.h"
#include "Repo.h"
#include "Doc.h"

int count=0;

void test_assert(bool condition, const std::string& message) {
	count++;
	if (!condition) {
		std::cerr << "Test "<< count <<" failed: " << message << std::endl;
	}else{
		std::cout << "Test "<< count <<" passed: " <<std::endl;
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
	test_assert(found != nullptr, "Media not found in category");//1

	// Downcast to Media to access counters/status APIs
	Media* m = dynamic_cast<Media*>(found);
	test_assert(m != nullptr, "Downcasting to Media");//2

    std::cout <<"PreTest: " << m->print() << std::endl;
	// Change status and update counts
	m->changeStatus("Ongoing");
	m->addCount();            // increment last counter
	m->addCount(0, 4);        // add 4 to first counter

	Category* foundCat = cat.cat("Anime");
	test_assert(foundCat != nullptr, "(Anime) Category not found");//3
	test_assert((foundCat->cat("Movie")==nullptr), "Retrieved Category does not match Anime");//4

	// Show different representational outputs
	std::string testPrintCat="![<@>Anime|{Episodes,Seasons}]\n";
	//std::cout<<"Anime print out:\n"<<cat.print()<<std::endl;
	test_assert(cat.print() == testPrintCat, "Category print representation mismatch");//5
	std::string testViewCat="<@>(Naruto){4,2}\\\\Ongoing\n";
	//std::cout<<"Anime view print out:\n"<<cat.view()<<std::endl;
	test_assert(cat.view() == testViewCat, "Category view representation mismatch");//6
	std::string testPrintMed="(Naruto){4,2}\\\\Ongoing";
	test_assert(m->print() == testPrintMed, "Media print representation mismatch");//7
    //Do not delete m
}

void addThemeStringTest(){
	Repo repo;

	// Test valid string input: ![<@>Anime|{Episodes,Seasons}]
	Category* cat = repo.addTheme("![<@>Anime|{Episodes,Seasons}]");
	test_assert(cat != nullptr, "addTheme(string) failed to parse valid input");

	// Verify the category was created with correct properties
	test_assert(cat->getSymbol() == "@", "addTheme(string) symbol mismatch");
	test_assert(cat->cat("Anime") != nullptr, "addTheme(string) title mismatch");
	std::vector<std::string> measures = cat->getMeasures();
	test_assert(measures.size() == 2, "addTheme(string) measures count mismatch");
	test_assert(measures[0] == "Episodes" && measures[1] == "Seasons", "addTheme(string) measures content mismatch");

	// Test malformed inputs return nullptr
	test_assert(repo.addTheme("InvalidInput") == nullptr, "addTheme(string) should reject missing '<'");
	test_assert(repo.addTheme("![<>Anime|{Episodes}]") == nullptr, "addTheme(string) should reject empty symbol");
	test_assert(repo.addTheme("![<@>|{Episodes}]") == nullptr, "addTheme(string) should reject missing title");
	test_assert(repo.addTheme("![<@>Anime|{}]") == nullptr, "addTheme(string) should reject empty measures");

	// Test print output matches expected format
	std::string expected = "![<@>Anime|{Episodes,Seasons}]\n";
	test_assert(cat->print() == expected, "addTheme(string) print output mismatch");
}

void RepoTest(){
	Repo repo;

	// Add a theme/category
	Category* cat = repo.addTheme("Anime", "@", {"Episodes", "Seasons"});
	repo.addTheme("Movie", "#", {"Duration"});
	test_assert(cat != nullptr, "Failed to add category");//8
	// Add media to the category
	Media* media = repo.addMedia("Anime", "(Naruto){0,0}\\\\Interested");
	repo.addMedia("Movie", "(Inception){148}\\\\Finished");
	std::vector<std::string> counters{"chapter","page"};
	Media* media2 = new Media(counters, "That time I got reincarnated as a slime");
	test_assert(media != nullptr, "Failed to add media to category");//9
	repo.addTheme("Manga", "@", counters);//Duplicate symbol test
	test_assert(repo.getTheme("Manga") == nullptr, "Added duplicate symbol category");//10
	repo.addTheme("Manga","+",counters);
	repo.addMedia("Manga", media2);
	test_assert(repo.getMedia("Manga", "That time I got reincarnated as a slime") == media2, "Failed to add media object to Manga category");//11

	// Retrieve media
	Media* retrieved = repo.getMedia("Anime", "Naruto");
	test_assert(retrieved != nullptr, "Failed to retrieve media from category");//12

	// Change media status and counts
	repo.status("Anime", "Naruto", "Ongoing");
	repo.changeMedia("Anime", "Naruto", 0, 4); // Add 4 to first counter
	retrieved->addCount(); // Increment last counter

	// Validate outputs
	std::cout<<"Repo Test Print: \n"<< repo.print() << std::endl;
}

void docTest(){
	Doc document;
	Repo* repo = document.getRepo();
	test_assert(repo != nullptr, "Failed to get Repo from Doc");//13

	Media* media=new Media(std::vector<std::string>{"Episodes","Seasons"}, "One Piece");
	Category* cat=repo->addTheme("Anime", "@", {"Episodes","Seasons"});
	//cat->add(media);
	Media* tester=repo->addMedia("Anime", media);
	test_assert(tester != nullptr, "Added duplicate media to category");//14
	cat=repo->addTheme("Movie", "#", {"Duration"});
	media=new Media(std::vector<std::string>{"Duration"}, "Inception");
	media->changeStatus("Finished");
	media->addCount(0,148);
	cat->add(media);
	repo->addTheme("Manga", "+", {"Chapters","Volumes"});
	repo->addTheme("Books", "*", {"Pages"});
	//media=new Media(std::vector<std::string>{"Chapters","Volumes"}, "Berserk");
	repo->addMedia("Manga", "(Berserk){364,40}\\\\Ongoing");
	//media=new Media(std::vector<std::string>{"Pages"}, "The Hobbit");
	repo->addMedia("Books", "(The Hobbit){310}\\\\Finished");

	document.save();

	Doc loadedDoc;
	Repo* loadedRepo=loadedDoc.getRepo();
	test_assert(loadedRepo != nullptr, "Failed to get Repo from loaded Doc");//15
	test_assert(loadedRepo->print() == repo->print(), "Loaded Repo content mismatch");//16
	std::cout<<"Original Repo Print: \n"<< repo->print() << std::endl;
	std::cout<<"Loaded Repo Print: \n"<< loadedRepo->print() << std::endl;
}
int main() {
	contentTest();
	addThemeStringTest();
	RepoTest();
	docTest();
	return 0;
}
