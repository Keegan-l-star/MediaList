#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "Doc.h"
#include "MedP.h"
#include "Repo.h"

TEST(ContentTest, MediaAndCategory) {
    std::vector<std::string> measures{"Episodes", "Seasons"};
    Category cat("Anime", "@", measures);
    cat.add("Naruto");

    MedP* found = cat.getChild("Naruto");
    ASSERT_NE(found, nullptr);

    Media* media = dynamic_cast<Media*>(found);
    ASSERT_NE(media, nullptr);

    media->changeStatus("Ongoing");
    media->addCount();
    media->addCount(0, 4);

    Category* foundCat = cat.cat("Anime");
    ASSERT_NE(foundCat, nullptr);
    EXPECT_EQ(foundCat->cat("Movie"), nullptr);
    EXPECT_EQ(cat.print(), "![<@>Anime|{Episodes,Seasons}]\n");
    EXPECT_EQ(cat.view(), "<@>(Naruto){4,2}\\\\Ongoing\n");
    EXPECT_EQ(media->print(), "(Naruto){4,2}\\\\Ongoing");
}

TEST(ThemeTest, AddThemeFromString) {
    Repo repo;
    Category* cat = repo.addTheme("![<@>Anime|{Episodes,Seasons}]");
    ASSERT_NE(cat, nullptr);

    EXPECT_EQ(cat->getSymbol(), "@");
    EXPECT_NE(cat->cat("Anime"), nullptr);

    std::vector<std::string> measures = cat->getMeasures();
    ASSERT_EQ(measures.size(), 2u);
    EXPECT_EQ(measures[0], "Episodes");
    EXPECT_EQ(measures[1], "Seasons");

    EXPECT_EQ(repo.addTheme("InvalidInput"), nullptr);
    EXPECT_EQ(repo.addTheme("![<>Anime|{Episodes}]"), nullptr);
    EXPECT_EQ(repo.addTheme("![<@>|{Episodes}]"), nullptr);
    EXPECT_EQ(repo.addTheme("![<@>Anime|{}]"), nullptr);
    EXPECT_EQ(cat->print(), "![<@>Anime|{Episodes,Seasons}]\n");
}

TEST(RepoTest, AddRetrieveAndUpdateMedia) {
    Repo repo;

    Category* cat = repo.addTheme("Anime", "@", {"Episodes", "Seasons"});
    repo.addTheme("Movie", "#", {"Duration"});
    ASSERT_NE(cat, nullptr);

    Media* media = repo.addMedia("Anime", "(Naruto){0,0}\\\\Interested");
    repo.addMedia("Movie", "(Inception){148}\\\\Finished");
    ASSERT_NE(media, nullptr);

    std::vector<std::string> counters{"chapter", "page"};
    Media* media2 = new Media(counters, "That time I got reincarnated as a slime");
    repo.addTheme("Manga", "@", counters);
    EXPECT_EQ(repo.getTheme("Manga"), nullptr);

    repo.addTheme("Manga", "+", counters);
    repo.addMedia("Manga", media2);
    EXPECT_EQ(repo.getMedia("Manga", "That time I got reincarnated as a slime"), media2);

    Media* retrieved = repo.getMedia("Anime", "Naruto");
    ASSERT_NE(retrieved, nullptr);

    repo.status("Anime", "Naruto", "Ongoing");
    repo.changeMedia("Anime", "Naruto", 0, 4);
    retrieved->addCount();
    EXPECT_EQ(retrieved->print(), "(Naruto){4,1}\\\\Ongoing");
}

TEST(DocTest, SaveAndLoadRepository) {
    Doc document;
    Repo* repo = document.getRepo();
    ASSERT_NE(repo, nullptr);

    Media* media = new Media(std::vector<std::string>{"Episodes", "Seasons"}, "One Piece");
    repo->addTheme("Anime", "@", {"Episodes", "Seasons"});
    EXPECT_NE(repo->addMedia("Anime", media), nullptr);

    Category* cat = repo->addTheme("Movie", "#", {"Duration"});
    media = new Media(std::vector<std::string>{"Duration"}, "Inception");
    media->changeStatus("Finished");
    media->addCount(0, 148);
    cat->add(media);

    repo->addTheme("Manga", "+", {"Chapters", "Volumes"});
    repo->addTheme("Books", "*", {"Pages"});
    repo->addMedia("Manga", "(Berserk){364,40}\\\\Ongoing");
    repo->addMedia("Books", "(The Hobbit){310}\\\\Finished");

    document.save();

    Doc loadedDoc;
    Repo* loadedRepo = loadedDoc.getRepo();
    ASSERT_NE(loadedRepo, nullptr);
    EXPECT_EQ(loadedRepo->print(), repo->print());
}
