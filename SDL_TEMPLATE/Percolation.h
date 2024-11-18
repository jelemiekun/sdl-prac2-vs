#pragma once
#include <SDL.h>

struct Site {
	unsigned int isOpen : 1;
	unsigned int isFull : 1;
	int xID;
	int yID;
	int size;
	SDL_Rect siteRect;
};

class Percolation {
private:
	Site** sites;
	int size;
	bool mPercolates;
	int numberOfOpenSites;

private:
	Site& findRoot(int x, int y);
	void makeUnion(Site& p, Site& q);

	void open(Site& siteID);
	bool isOpen(const Site& siteID);
	bool isFull(Site& siteID);


public:
	Percolation(const int n, const int resSize);
	~Percolation();
	void openRandomSite();
	int getNumberOfOpenSites();
	bool percolates();
	void render(SDL_Renderer*& gRenderer);
};

