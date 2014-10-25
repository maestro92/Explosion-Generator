#include "hgrid.h"

#include "time.h"
#include "particle.h"

#ifdef USE_OPENGL
#include "opengl.h"
#endif

HGrid* hgrid;

HGrid::HGrid(float max_cell_size, int max_level)
	: MAX_CELL_SIZE(max_cell_size), MAX_LEVEL(max_level) {
	assert(MAX_LEVEL <= 32);
	for (int i = 0; i < 32; i++)
		particles_at_level[i] = 0;
	occupied_level_mask = 0;
}

void HGrid::insert(Particle* p) {
	insert(p, make_hash(p));
}

void HGrid::insert(Particle* p, hash_t hash) {
	p->set_hash(hash.value);
	particles_at_level[hash.data.level]++;
	occupied_level_mask |= (1 << hash.data.level);

	if (grid.find(hash.value) == grid.end()) {
		p->set_prev(NULL);
		p->set_next(NULL);
		grid[hash.value] = p;
	} else {
		Particle* head = grid[hash.value];
		head->set_prev(p);
		p->set_next(head);
		p->set_prev(NULL);
		grid[hash.value] = p;
	}
}

void HGrid::erase(const Particle* p) {
	hash_t hash; hash.value = p->hash();

	if (--particles_at_level[hash.data.level] == 0)
		occupied_level_mask &= ~(1 << hash.data.level);

	Particle* prev = p->prev();
	Particle* next = p->next();

	if (prev) {
		if (next) {
			prev->set_next(next);
			next->set_prev(prev);
		} else {
			prev->set_next(NULL);
		}
	} else {
		if (next) {
			grid[hash.value] = next;
			next->set_prev(NULL);
		} else {
			grid.erase(hash.value);
		}
	}
}

void HGrid::rehash(Particle* p) {
	erase(p); insert(p);
#ifdef DEBUG_HGRID
	hash_t hash  = make_hash(p), phash = p->hash();
	fprintf(stderr, "rehash: [%d: %d %d %d] -> [%d: %d %d %d]\n"
		"p = (%f, %f, %f), v = (%f, %f, %f), size = %f (%f)\n",
		phash.data.level, phash.data.x, phash.data.y, phash.data.z,
		hash.data.level,  hash.data.x,  hash.data.y,  hash.data.z,
		x[0], x[1], x[2], v[0], v[1], v[2], size, MAX_CELL_SIZE / (1 << hash.data.level));
#endif
}

float HGrid::next_rehash_time(const Particle* p) const {
	hash_t hash;
	hash.value = p->hash();
	Vector x = p->position();
	Vector v = p->velocity();

	float s  = MAX_CELL_SIZE / (1 << hash.data.level);
    float tx = abs(((v[0] >= 0.0 ? (hash.data.x+1) : hash.data.x ) * s - x[0]) / v[0]);
    float ty = abs(((v[1] >= 0.0 ? (hash.data.y+1) : hash.data.y ) * s - x[1]) / v[1]);
    float tz = abs(((v[2] >= 0.0 ? (hash.data.z+1) : hash.data.z ) * s - x[2]) / v[2]);

	float t_level  = s / (2.0 * p->bounding_radius());
	float t_escape = t_curr + min(min(tx,ty),tz);

#ifdef DEBUG_HGRID
	fprintf(stderr, "HGRID: %p: tx = %.9f, ty = %.9f, tz = %.9f\n", p, tx, ty, tz);
	fprintf(stderr, "t_curr = %.9f, t_escape = %.9f t_level = %.9f\n", t_curr, t_escape, t_level);
#endif

	return max(t_curr, min(t_level, t_escape));
}

#ifdef USE_OPENGL
void HGrid::draw() const {
    for (auto it = grid.begin(); it != grid.end(); it++) 
    {
		hash_t hash; hash.value = (*it).first;

		float size = MAX_CELL_SIZE / (1 << hash.data.level);

        Vector center;
        center[0] = (hash.data.x + 0.5) * size;
        center[1] = (hash.data.y + 0.5) * size;
        center[2] = (hash.data.z + 0.5) * size;

        glPushMatrix();
        glTranslated(center[0], center[1], center[2]);
        glutWireCube(size);
        glPopMatrix();
    }
}
#endif

void HGrid::find_neighbors(const Particle* p, std::vector<Particle*>& neighbors) {
	hash_t hash; unsigned int mask = occupied_level_mask;
	Vector x = p->position();

	for (unsigned int level = 0; level <= MAX_LEVEL; mask >>=1, level++) {
		if (mask == 0) return; /* no more occupied levels to check */

		if ((mask & 1) == 0) continue; /* level is not occupied */

		float cell_size = MAX_CELL_SIZE / (1 << level);
		float delta = p->bounding_radius(t_curr) + cell_size/2.0 + EPSILON;

		hash.data.level = level;

		short int imin = (short int) floor((x[0] - delta) / cell_size);
		short int jmin = (short int) floor((x[1] - delta) / cell_size);
		short int kmin = (short int) floor((x[2] - delta) / cell_size);

		short int imax = (short int)  ceil((x[0] + delta) / cell_size);
		short int jmax = (short int)  ceil((x[1] + delta) / cell_size);
		short int kmax = (short int)  ceil((x[2] + delta) / cell_size);

		for (short int i = imin; i < imax; i++) {
			hash.data.x = i;
			for (short int j = jmin; j < jmax; j++) {
				hash.data.y = j;
				for (short int k = kmin; k < kmax; k++) {
					hash.data.z = k;

					if (grid.find(hash.value) != grid.end()) {
						Particle *neighbor = grid[hash.value];

						while(neighbor != NULL) {
							if (neighbor != p)
								neighbors.push_back(neighbor);
							neighbor = neighbor->next();
						}
					}
				}
			}
		}
	}
}

hash_t HGrid::make_hash(const Point& p, float size) 
{
	hash_t hash; unsigned short int level = MAX_LEVEL;

	assert(size < MAX_CELL_SIZE);

	while (size > MAX_CELL_SIZE / (1 << level)) level--;

	float cell_size = MAX_CELL_SIZE / (1 << level);

	hash.data.x = (short int) floor(p[0] / cell_size);
	hash.data.y = (short int) floor(p[1] / cell_size);
	hash.data.z = (short int) floor(p[2] / cell_size);
	hash.data.level = level;

	return hash;
}

hash_t HGrid::make_hash(Particle* p) {
	float size = 2.001 * p->bounding_radius(t_curr);
	Vector v = p->velocity(); float speed = norm(v);
	Point  x = p->position() + (speed > EPSILON ? 0.01 * size * v/speed : v);
	return make_hash(x, size);
}
