#include "StoryMode.hpp"

#include "Sprite.hpp"
#include "DrawSprites.hpp"
#include "Load.hpp"
#include "data_path.hpp"
#include "gl_errors.hpp"
#include "MenuMode.hpp"

Sprite const *sprite_main_hall = nullptr;
Sprite const *sprite_room_red = nullptr;
Sprite const *sprite_room_pink = nullptr;
Sprite const *sprite_room_yellow = nullptr;

const std::string text_main_hall = "You stand in the Hall of the Haruspices.";
const std::string text_main_hall_2 = "They must be appeased.";
const std::string text_main_hall_done = "The Hall has grown calm. Your job is done.";
const std::string text_main_hall_leave_not_done = "There is still hunger here.";

const std::string text_choice_red = "Squeeze past the red gap";
const std::string text_choice_pink = "Push through the pink doors";
const std::string text_choice_yellow = "Descend into the yellow hole";

const std::string text_red = "The Chamber of Hearts roars. \"I HAVE ";
const std::string text_red_2 ="NEED. LORE OF FLAMES. \" ";
const std::string text_red_success = "The Chamber begins to pulse ";
const std::string text_red_success_2 = "rhythmically. \"GOOD.\" ";
const std::string text_red_fail = "The engine of hate is ";
const std::string text_red_fail_2 = "stirring. \"NO. NO NO NO.\" ";
const std::string text_pink = "The Augur of Time towers. \"I must consume ";
const std::string text_pink_2 = "that which feasts on the heavens.\" ";
const std::string text_pink_success = "The sunflower floats into the Augur's";
const std::string text_pink_success_2 = " chest. \"Delicious. You may leave.\" ";
const std::string text_pink_fail = "Time slows. \"Are you trying to insult me?.\" ";
const std::string text_yellow = "The Eternal Horror shambles towards you. ";
const std::string text_yellow_2 = "\"Grantt... me a f-facsimile of l-life.\" ";
const std::string text_yellow_success = "The toy is absorbed. \"B-beautiful.\" ";
const std::string text_yellow_fail = "The air is filled with the smell of";
const std::string text_yellow_fail_2 = " sulfur. \"N-no... \" ";
const std::string text_done = "You have fulfilled your task in here.";

const std::string text_gift_red = "Give Muffin Cookbook";
const std::string text_gift_pink = "Give Sunflower";
const std::string text_gift_yellow = "Give Dog Plush Toy";
const std::string text_leave = "Leave";

Load< SpriteAtlas > text( LoadTagDefault, []() -> SpriteAtlas const * {
	SpriteAtlas const *ret = new SpriteAtlas( data_path( "the-planet" ) );
	return ret;
} );

Load< SpriteAtlas > sprites(LoadTagDefault, []() -> SpriteAtlas const * {
	SpriteAtlas const *ret = new SpriteAtlas(data_path("the-hall"));
	sprite_main_hall = &ret->lookup( "main-hall" );
	sprite_room_red = &ret->lookup( "red-room" );
	sprite_room_pink = &ret->lookup( "pink-room" );
	sprite_room_yellow = &ret->lookup( "yellow-room" );
	return ret;
});

StoryMode::StoryMode() {
}

StoryMode::~StoryMode() {
}

bool StoryMode::handle_event(SDL_Event const &, glm::uvec2 const &window_size) {
	if (Mode::current.get() != this) return false;

	return false;
}

void StoryMode::update(float elapsed) {
	if (Mode::current.get() == this) {
		//there is no menu displayed! Make one:
		enter_scene();
	}
}

void StoryMode::enter_scene() {
	//just entered this scene, adjust flags and build menu as appropriate:
	std::vector< MenuMode::Item > items;
	glm::vec2 at(3.0f, view_max.y - 16.0f);
	auto add_text = [&items,&at](std::string text) {
		items.emplace_back(text, nullptr, 1.0f, nullptr, at);
		at.y -= 20.0f;
	};
	auto add_choice = [&items,&at]( std::string text, std::function< void(MenuMode::Item const &) > const &fn) {
		items.emplace_back(text, nullptr, 1.0f, fn, at + glm::vec2(8.0f, 0.0f));
		at.y -= 20.0f;
	};

	//Set 
	


	if( location == Hall )
	{
		if( gave_red && gave_yellow && gave_pink )
		{
			add_text( text_main_hall_done );
			add_choice( text_leave, [this]( MenuMode::Item const & ){
				Mode::current = nullptr;
			} );
		}
		else
		{
			add_text( text_main_hall );
			add_text( text_main_hall_2 );
		}

		at.y -= 20.0f;

		if(!gave_red )
		add_choice( text_choice_red, [this]( MenuMode::Item const & ){
			location = RedRoom;
			anger = false;
			Mode::current = shared_from_this();
		} );
		if(!gave_pink )
		add_choice( text_choice_pink, [this]( MenuMode::Item const & ){
			location = PinkRoom;
			anger = false;
			Mode::current = shared_from_this();
		} );
		if(!gave_yellow )
		add_choice( text_choice_yellow, [this]( MenuMode::Item const & ){
			location = YellowRoom;
			anger = false;
			Mode::current = shared_from_this();
		} );
	}
	else
	{
		bool success = false;
		if( location == RedRoom )
		{
			if( gave_red ) 
			{ 
				success = true;
				add_text( text_red_success ); 
				add_text( text_red_success_2 );
			}
			else if( anger ) 
			{
				add_text( text_red_fail ); 
				add_text( text_red_fail_2 ); 
			}
			else 
			{
				add_text( text_red ); 
				add_text( text_red_2 );
			}
		}
		else if( location == PinkRoom )
		{
			if( gave_pink )
			{
				success = true;
				add_text( text_pink_success );
				add_text( text_pink_success_2 );
			}
			else if( anger )
			{
				add_text( text_pink_fail );
			}
			else
			{
				add_text( text_pink );
				add_text( text_pink_2 );
			}
		}
		else if( location == YellowRoom )
		{
			if( gave_yellow )
			{
				success = true;
				add_text( text_yellow_success );
			}
			else if( anger )
			{
				add_text( text_yellow_fail );
				add_text( text_yellow_fail_2 );
			}
			else
			{
				add_text( text_yellow );
				add_text( text_yellow_2 );
			}
		}

		at.y -= 20.0f;

		if( !gave_red && !success)
		{
			add_choice( text_gift_red, [this]( MenuMode::Item const & ){
				anger = location != RedRoom;
				gave_red = location == RedRoom;
				Mode::current = shared_from_this();
			} );
		}
		if( !gave_pink && !success )
		{
			add_choice( text_gift_pink, [this]( MenuMode::Item const & ){
				anger = location != PinkRoom;
				gave_pink = location == PinkRoom;
				Mode::current = shared_from_this();
			} );
		}
		if( !gave_yellow && !success )
		{
			add_choice( text_gift_yellow, [this]( MenuMode::Item const & ){
				anger = location != YellowRoom;
				gave_yellow = location == YellowRoom;
				Mode::current = shared_from_this();
			} );
		}
		add_choice( text_leave, [this]( MenuMode::Item const & ){
			location = Hall;
			Mode::current = shared_from_this();
		} );
	}

	/*if (location == Dunes) {
		if (dunes.wont_leave) {
			dunes.wont_leave = false;
			add_text(text_dunes_wont_leave);
		}
		if (dunes.first_visit) {
			dunes.first_visit = false;
			add_text(text_dunes_landing);
		} else {
			add_text(text_dunes_return);
		}
		at.y -= 2.0f; //gap before choices
		add_choice(text_dunes_do_walk_west, [this](MenuMode::Item const &){
			location = Hill;
			Mode::current = shared_from_this();
		});
		add_choice(text_dunes_do_walk_east, [this](MenuMode::Item const &){
			location = Oasis;
			Mode::current = shared_from_this();
		});
		if (!dunes.first_visit) {
			add_choice(text_dunes_do_leave, [this](MenuMode::Item const &){
				if (added_stone) {
					//TODO: some sort of victory animation?
					Mode::current = nullptr;
				} else {
					dunes.wont_leave = true;
					Mode::current = shared_from_this();
				}
			});
		}
	} else if (location == Oasis) {
		if (oasis.took_stone) {
			oasis.took_stone = false;
			add_text(text_oasis_stone_taken);
		}
		if (oasis.first_visit) {
			oasis.first_visit = false;
			add_text(text_oasis_intro);
		} else {
			add_text(text_oasis_plain);
		}
		if (!have_stone) {
			add_text(text_oasis_stone);
		}
		at.y -= 2.0f; //gap before choices
		if (!have_stone) {
			add_choice(text_oasis_do_take_stone, [this](MenuMode::Item const &){
				have_stone = true;
				oasis.took_stone = true;
				Mode::current = shared_from_this();
			});
		}
		add_choice(text_oasis_do_return, [this](MenuMode::Item const &){
			location = Dunes;
			Mode::current = shared_from_this();
		});
	} else if (location == Hill) {
		if (hill.added_stone) {
			hill.added_stone = false;
			add_text(text_hill_stone_added);
		}
		if (hill.first_visit) {
			hill.first_visit = false;
			add_text(text_hill_intro);
		} else {
			if (added_stone) {
				add_text(text_hill_active);
			} else {
				add_text(text_hill_inactive);
			}
		}
		at.y -= 2.0f; //gap before choices
		if (have_stone && !added_stone) {
			add_choice(text_hill_do_add_stone, [this](MenuMode::Item const &){
				added_stone = true;
				hill.added_stone = true;
				Mode::current = shared_from_this();
			});
		}
		add_choice(text_hill_do_return, [this](MenuMode::Item const &){
			location = Dunes;
			Mode::current = shared_from_this();
		});
	}*/
	std::shared_ptr< MenuMode > menu = std::make_shared< MenuMode >(items);
	menu->atlas = text;
	menu->view_min = view_min;
	menu->view_max = view_max;
	menu->background = shared_from_this();
	Mode::current = menu;
}

void StoryMode::draw(glm::uvec2 const &drawable_size) {
	//clear the color buffer:
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//use alpha blending:
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//don't use the depth test:
	glDisable(GL_DEPTH_TEST);

	{ //use a DrawSprites to do the drawing:
		DrawSprites draw(*sprites, view_min, view_max, drawable_size, DrawSprites::AlignPixelPerfect);
		glm::vec2 ul = glm::vec2(view_min.x, view_max.y);
		if (location == Hall) {
			draw.draw(*sprite_main_hall, ul);
		} else if (location == RedRoom) {
			draw.draw(*sprite_room_red, ul);
		} else if (location == YellowRoom) {
			draw.draw(*sprite_room_yellow, ul);
		} else if( location == PinkRoom ) {
			draw.draw( *sprite_room_pink, ul );
		}
	}
	GL_ERRORS(); //did the DrawSprites do something wrong?
}
