#ifndef _HGRID_H
#define _HGRID_H

#include <vector>
#include <unordered_map>

#include "scalar.h"

class Point;
class Particle;

typedef union {
	unsigned long int value;
	struct {
		short int x, y, z;
		unsigned short int level;
	} data;
} hash_t;

class HGrid {
  public:

	/* MAX_CELL_SIZE must be chosen to be larger than the largest particle
	 * diameter at the end of the simulation, otherwise big problems will happen. */

	/* MAX_LEVEL must be less than or equal 32, because bit shift operations only
	 * work for 32 bits or less. However, 8 levels should be more than enough in
	 * most cases. Only runs with millions of particles will need more than 8 levels. */

	HGrid(float max_cell_size = 2.0, int max_level = 8);
	~HGrid() { grid.clear(); }

	void insert(Particle* p);
	void insert(Particle* p, hash_t hash);
	void erase(const Particle* p);
	void rehash(Particle* p);
	float next_rehash_time(const Particle* p) const;
	void find_neighbors(const Particle* p, std::vector<Particle*>& neighbors);

#ifdef USE_OPENGL
	void draw() const;
#endif

  private:
	const float MAX_CELL_SIZE;
	const unsigned int MAX_LEVEL;
	unsigned long int particles_at_level[32];
	unsigned int occupied_level_mask; /* bit mask of occupied levels */
	std::unordered_map<unsigned long int, Particle*> grid;

	hash_t make_hash(Particle* p);
	hash_t make_hash(const Point& p, float size);
};

extern HGrid* hgrid;

#endif

