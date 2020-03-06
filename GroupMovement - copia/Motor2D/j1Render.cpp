#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "Brofiler/Brofiler.h"

#define VSYNC true

j1Render::j1Render() : j1Module()
{
	name.create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
j1Render::~j1Render()
{}

// Called before render is available
bool j1Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");

	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		App->vsync.create("On");
		LOG("Using vsync");

	}
	else {
		App->vsync.create("Off");
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());

		ret = false;
	}
	else
	{
		camera.w = App->win->screen_surface->w;
		camera.h = App->win->screen_surface->h;
		camera.x = 0;
		camera.y = 0;
	}
	camera_offset.x = 0;
	camera_offset.y = 0;
	return ret;
}

// Called before the first frame
bool j1Render::Start()
{
	LOG("render start");

	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	
	return true;
}

// Called each loop iteration
bool j1Render::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_Render", Profiler::Color::Aqua);

	SDL_RenderClear(renderer);
	return true;
}

bool j1Render::Update(float dt)
{
	BROFILER_CATEGORY("Update_Render", Profiler::Color::Aquamarine);

	if (shaking)
		UpdateCameraShake();

	return true;
}

bool j1Render::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Post_Render", Profiler::Color::Azure)

	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool j1Render::CleanUp()
{
	LOG("Destroying SDL render");

	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool j1Render::Load(pugi::xml_node& data)
{
	return true;
}

// Save Game State
bool j1Render::Save(pugi::xml_node& data) const
{
	return true;
}

void j1Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void j1Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void j1Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool j1Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speedX, float speedY, SDL_RendererFlip flip, double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speedX + camera_offset.x) + x * scale;
	rect.y = (int)(camera.y * speedY + camera_offset.y) + y * scale;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
	//	LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
	//	App->console->write("Cannot blit to screen. SDL_RenderCopy error ");

		ret = false;
	}

	return ret;
}

bool j1Render::BlitWithScale(SDL_Texture* texture, int x, int y, SDL_Rect* _section, float direction, float speed, float fillAmount, RENDER_PIVOT pivot)
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_Rect rect;
	SDL_Rect section = *_section;
	int w = section.w - section.w * fillAmount;

	section.w -= w;

	switch (pivot)
	{
	case TOP_RIGHT:
		rect.x = (int)(camera.x * speed) + (x - section.w) * scale;
		rect.y = (int)(camera.y * speed) + y * scale;
		break;
	case TOP_LEFT:
		rect.x = (int)(camera.x * speed) + (x)* scale;
		rect.y = (int)(camera.y * speed) + y * scale;
		break;
	case MIDDLE:
		rect.x = (int)(camera.x * speed) + (x + w + section.x / 2) * scale;
		rect.y = (int)(camera.y * speed) + (y + section.y / 2) * scale;
		break;
	}

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	if (direction < 0)
	{
		direction = fabsf(direction);
		flip = SDL_FLIP_HORIZONTAL;
	}

	if (&section != NULL)
	{
		rect.w = section.w * direction;
		rect.h = section.h * direction;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	if (SDL_RenderCopyEx(renderer, texture, &section, &rect, 0, NULL, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());

		ret = false;
	}

	return ret;

}

bool j1Render::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());

		ret = false;
	}

	return ret;
}

bool j1Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());

		ret = false;
	}

	return ret;
}

bool j1Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());

		ret = false;
	}

	return ret;
}


void j1Render::StartCameraShake(int duration, float magnitude)
{
	//Store the data and start the shake
	shake_magnitude = magnitude;
	shake_duration = duration;
	shaking = true;
	shake_timer = SDL_GetTicks();
}

void j1Render::UpdateCameraShake()
{
	
	// Generate a random value and set the camera offset

	//Update the shake timer, stop shaking if we reach the full duration
	//Generate a random value and set the camera offset

	if (SDL_GetTicks() - shake_timer < shake_duration) {

		camera_offset.x = -(int)shake_magnitude + rand() % (int)shake_magnitude;
		camera_offset.y = -(int)shake_magnitude + rand() % (int)shake_magnitude;

	}
	else {
		camera_offset.x = 0;
		camera_offset.y = 0;
		shaking = false;
	}

}
iPoint j1Render::ScreenToWorld(int x, int y) const
{
	iPoint ret;
	int scale = App->win->GetScale();

	ret.x = (x - camera.x / scale);
	ret.y = (y - camera.y / scale);

	return ret;
}