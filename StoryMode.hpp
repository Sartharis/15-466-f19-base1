
/*
 * StoryMode implements a story about The Planet of Choices.
 *
 */

#include "Mode.hpp"

struct StoryMode : Mode {
	StoryMode();
	virtual ~StoryMode();

	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//called to create menu for current scene:
	void enter_scene();

	//------ story state -------
	enum {
		Hall,
		RedRoom,
		YellowRoom,
		PinkRoom
	} location = Hall;
	bool gave_red = false;
	bool gave_yellow = false;
	bool gave_pink = false;
	bool anger = false;
	
	glm::vec2 view_min = glm::vec2(0,0);
	glm::vec2 view_max = glm::vec2(256, 224);
};
