#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <iostream>

#include "Mesh.h"
#include "Geometry.h"

static int read_line(FILE *infp, char *line, int len)
{
retry:
	if (fgets(line, len, infp) == NULL)
		return -1; // end of file

	size_t read_len = strlen(line);
	if (read_len == 0)
		goto retry;

	// trim crlf
	for(;;) {
		if (line[read_len-1] == '\r' || line[read_len - 1] == '\n') {
			line[read_len-1] = 0;
			read_len--;
			if (read_len == 0)
				goto retry;
		} else {
			break;
		}
	}

	return (int)read_len;
}

static int tokenize(char *str, const char token, char **tokens, int num_tokens)
{
	int pos = 0;
	int curr_token = -1;
	enum {
		IN_SPACE,
		IN_TOKEN
	} state = IN_SPACE;

	for(;;) {
		if (str[pos] == 0)
			break;

		switch (state) {
			case IN_SPACE:
				if (str[pos] != token) {
					// transition out of space and start a new token
					tokens[++curr_token] = &str[pos];
					state = IN_TOKEN;
				} else {
					// null all spaces
					str[pos] = 0;
					pos++;
				}
				break;
			case IN_TOKEN:
				if (str[pos] == token) {
					state = IN_SPACE;
				} else {
					pos++;
				}
				break;
		}
	}

	return curr_token + 1;
}

int obj_load(FILE *infp, Geometry **new_geometry)
{
	Geometry *g;
	Mesh *mesh = 0;
	char line[1024];
	bool addedSurfaces = true; // have we started adding surfaces in the current mesh?
	int surfacenum = 0;

	g = new Geometry;

	std::string nextname = "default0";
	for (;;) {
		int len = read_line(infp, line, sizeof(line));
		if (len < 0)
			break;

		if (strncmp(line, "v ", 2) == 0) {
			if (addedSurfaces) {
				// we have previously been adding surfaces. The start of
				// more vertexes is a sign we need a new mesh
				if (mesh) {
					g->AddMesh(mesh);
				}
				mesh = g->CreateMesh();
				addedSurfaces = false;
				surfacenum++;
				mesh->SetName(nextname);
				char tempname[512];
				sprintf(tempname, "default%d", surfacenum);
				nextname = tempname;
			}

			Vertex v;
			sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
			g->AddVert(v);
		} else if (strncmp(line, "vn ", 3) == 0) {
			Vertex v;
			sscanf(line, "vn %f %f %f", &v.x, &v.y, &v.z);
			g->AddNormalVert(v);
		} else if (strncmp(line, "vp ", 3) == 0) {
			// point in space
			// consume them for now
		} else if (strncmp(line, "vt ", 3) == 0) {
			Vertex v;
			sscanf(line, "vt %f %f", &v.u, &v.v);
			g->AddUVVert(v);
		} else if (strncmp(line, "f ", 2) == 0) {
			addedSurfaces = true;

			// a surface
			Surface *s = new Surface;

			// tokenize the line to pick out the indexes
			char *tokens[16];
			int num_tokens = tokenize(line, ' ', tokens, 16);
			
			// iterate over each x/y/z triplet
			for (int index = 1; index < num_tokens; index++) {
				char *subtokens[16];
				int num_subtokens = tokenize(tokens[index], '/', subtokens, 16);

				int posindex, normindex, uvindex;
				posindex = normindex = uvindex = 0;

				if (num_subtokens > 0)
					posindex = atoi(subtokens[0]);
				if (num_subtokens > 1)
					uvindex = atoi(subtokens[1]);
				if (num_subtokens > 2)
					normindex = atoi(subtokens[2]);
				s->AddIndex(posindex - 1, normindex - 1, uvindex - 1);
			}

			mesh->AddSurface(s);
		} else if (strncmp(line, "mtllib", 6) == 0) {
			// eat these lines
		} else if (strncmp(line, "usemtl", 6) == 0) {
			// eat these lines
		} else if (strncmp(line, "g ", 2) == 0) {
			// eat these lines
		} else if (strncmp(line, "s ", 2) == 0) {
			// eat these lines
		} else if (strncmp(line, "l ", 2) == 0) {
			// eat these lines
		} else if (strncmp(line, "#  ExternalName: ", 17) == 0) {
			nextname = line + 17;
		} else if (line[0] == '#') {
			// consume all other comment lines
		} else {
			// unknown
			std::cerr << "unknown .obj file line '" << line << "'" << std::endl;
//			assert(0);
		}
	}

	// add the last mesh to our geometry
	if (mesh && addedSurfaces) {
		g->AddMesh(mesh);
	}

	*new_geometry = g;

	return 0;
}
