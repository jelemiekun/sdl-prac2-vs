#include "Percolation.h"
#include <iostream>
#include <random>

Percolation::Percolation(const int n, const int resSize)
	: sites(nullptr), mPercolates(false), numberOfOpenSites(0) {
	if (n <= 0) {
		std::cout << "Size must be greater than zero!" << '\n';
	}

	size = n;

	// Initialize 2D array sitesID
	sites = new Site*[size];
	for (int i = 0; i < size; i++) {
		sites[i] = new Site[size];
	}

	// Make all sitesID to 0
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			sites[i][j].isOpen = 0;
			sites[i][j].isFull = 0;

			sites[i][j].xID = i;
			sites[i][j].yID = j;

			sites[i][j].size = 1;

			sites[i][j].siteRect.x = (resSize / size) * j;
			sites[i][j].siteRect.y = (resSize / size) * i;
			sites[i][j].siteRect.w = (resSize / size);
			sites[i][j].siteRect.h = (resSize / size);
		}
	}
}

Percolation::~Percolation() {
	for (int i = 0; i < size; i++) {
		delete[] sites[i];
	}

	delete sites;
}

bool Percolation::isOpen(const Site& siteID) {
	return siteID.isOpen;
}

bool Percolation::isFull(Site& siteID) {
	Site* tempSiteXZero = &findRoot(siteID.xID, siteID.yID);
	
	return (tempSiteXZero->isOpen && tempSiteXZero->isFull);
}

void Percolation::open(Site& siteID) {
	if (!isOpen(siteID)) {
		++numberOfOpenSites;

		if (siteID.xID == 0) siteID.isFull = 1;

		siteID.isOpen = 1;

		int x = siteID.xID;
		int y = siteID.yID;

		if (y > 0)			makeUnion(siteID, sites[x][y - 1]);
		if (y < size - 1)	makeUnion(siteID, sites[x][y + 1]);
		if (x > 0)			makeUnion(siteID, sites[x - 1][y]);
		if (x < size - 1)	makeUnion(siteID, sites[x + 1][y]);
	}
}

Site& Percolation::findRoot(int x, int y) {
	Site* temp = &sites[x][y];

	while (x != temp->xID || y != temp->yID) {
		int originalX = x;
		int originalY = y;

		x = temp->xID;
		y = temp->yID;

		temp = &sites[x][y];

		// Path compression
		sites[originalX][originalY].xID = x;
		sites[originalX][originalY].yID = y;
	}

	return *temp;
}


void Percolation::makeUnion(Site& p, Site& q) {
	if (q.isOpen) {
		Site& rootP = findRoot(p.xID, p.yID);
		Site& rootQ = findRoot(q.xID, q.yID);

		if (rootP.size < rootQ.size) {
			rootP.xID = rootQ.xID;
			rootP.yID = rootQ.yID;
			rootQ.size += rootP.size;

			if (rootP.isFull || rootQ.isFull) {
				rootQ.isFull = true;
			}
		} else {
			rootQ.xID = rootP.xID;
			rootQ.yID = rootP.yID;
			rootP.size += rootQ.size;

			if (rootP.isFull || rootQ.isFull) {
				rootP.isFull = true;
			}
		}
	}
}


void Percolation::openRandomSite() {
	if (!percolates()) {
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist(0, size - 1);

		int x = (dist(rng));
		int y = dist(rng);

		open(sites[x][y]);
	}
}

int Percolation::getNumberOfOpenSites() {
	return numberOfOpenSites;
}


bool Percolation::percolates() {
	if (!mPercolates) {
		for (int i = 0; i < size; i++) {
			Site* topRoot = &findRoot(0, i);
			for (int j = 0; j < size; j++) {
				Site* bottomRoot = &findRoot(size - 1, j);

				if (topRoot->xID == bottomRoot->xID && topRoot->yID == bottomRoot->yID) {
					mPercolates = true;
					return mPercolates;
				}
			}
		}
	}

	return mPercolates;
}


void Percolation::render(SDL_Renderer*& gRenderer) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (isOpen(sites[i][j])) {
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderFillRect(gRenderer, &sites[i][j].siteRect);
			} else {
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
				SDL_RenderFillRect(gRenderer, &sites[i][j].siteRect);
			}

			if (isFull(sites[i][j])) {
				SDL_SetRenderDrawColor(gRenderer, 51, 129, 255, 255);
				SDL_RenderFillRect(gRenderer, &sites[i][j].siteRect);
			} 

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(gRenderer, &sites[i][j].siteRect);
		}
	}
}