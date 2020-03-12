#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Audio.h"
#include <math.h>
#include "Brofiler/Brofiler.h"



j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor ----------------------------------------------
j1Map::~j1Map()
{}

// Called before render is available ----------------------------------------------
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;
	folder.create(config.child("folder").child_value());
	return ret;
}

//Blit map
void j1Map::Draw()
{
	BROFILER_CATEGORY("Map_Draw", Profiler::Color::DarkKhaki)

	if(map_loaded == false)
		return;

	
	MapLayer* it;
	for (list<MapLayer*>::const_iterator layer = data.layers.begin(); layer != data.layers.end(); ++layer) {
			it = *layer;
		parallax = it->returnPropfValue("Parallax");
		for(int y = 0; y < data.height; ++y)
		{
			for(int x = 0; x < data.width; ++x)
			{
				int tile_id = it->Get(x, y);
				if(tile_id > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);
					if (tileset != nullptr)
					{
						SDL_Rect r = tileset->GetTileRect(tile_id);
						iPoint pos = MapToWorld(x, y);
						if( it->returnPropValue("Nodraw")==0  || blitColliders ){
						//Blit every tile inside camera limits and colliders if blitcolliders is active ----------------------------------------------
							if (pos.x >= -1 * ((App->render->camera.x+64)) *parallax && pos.y >= -1 * (App->render->camera.y+32)) {
								if (pos.x <= -1 * (App->render->camera.x)*parallax + App->win->width && pos.y <= -1 * (App->render->camera.y-32) + App->win->height) {
									App->render->Blit(tileset->texture, pos.x, pos.y, &r,parallax); 
								}
							}
						}
					}
				}
			}
		}
	}
}

//Return a tileset depending on which tile id we give ----------------------------------------------
TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	list<TileSet*>::const_iterator item = data.tilesets.begin();
	TileSet* tileset = *item;
	while (item != data.tilesets.end())
	{
		if (id >= (*item)->firstgid) {
			tileset = *item;
			item++;

		}
		else {
			item++;
		}
	}

	return tileset;
}

//Convert map coord to world ----------------------------------------------
iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tile_width * 0.5f);
		ret.y = (x + y) * (data.tile_height * 0.5f);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

//Convert world coord to map coord ----------------------------------------------
iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0,0);

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{
		
		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.x = int( (x / half_width + y / half_height) / 2);
		ret.y = int( (y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");

		ret.x = x; ret.y = y;
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting ----------------------------------------------
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets ----------------------------------------------
	
	list<TileSet*>::iterator TileSet_list;
	TileSet* TileSet_it;

	for (TileSet_list = data.tilesets.begin(); TileSet_list != data.tilesets.end(); ++TileSet_list) {
		TileSet_it = *TileSet_list;
		App->tex->UnLoad(TileSet_it->texture);
		RELEASE(TileSet_it);
	}
	
	data.tilesets.clear();

	// Remove all layers ----------------------------------------------
	list<MapLayer*>::iterator Layer_list;
	MapLayer* Layer_it;

	for (Layer_list = data.layers.begin(); Layer_list != data.layers.end(); ++Layer_list) {
		Layer_it = *Layer_list;
		RELEASE(Layer_it->data);
	}
	data.layers.clear();

	// Clean up the pugui tree ----------------------------------------------
	map_file.reset();

	return true;
}

// Load new map, and fill some lists with data extracted from functions below ----------------------------------------------
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());
	

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());

		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.push_back(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for(layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if(ret == true)
			data.layers.push_back(lay);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);

		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		

		list<TileSet*>::const_iterator item_tileset = data.tilesets.begin();

		while (item_tileset != data.tilesets.end())
		{
			TileSet* s = *item_tileset;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item_tileset++;
		}

		list<MapLayer*>::const_iterator item_layer = data.layers.begin();

	
		while(item_layer != data.layers.end())
		{
			MapLayer* l = *item_layer;

		
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			//Uncomment if want to see properties on output window (does not work with strings)
			/*for (int i = 0; i < MAX_PROPERTIES; i++) {
				LOG("Prop name: %s  value %d", l->property[i].name.GetString(), l->property[i].prop.ivalue);
			}
			*/

			item_layer++;
		}
	}
	
	map_loaded = ret;

	return ret;
}

// Load map general properties ----------------------------------------------
bool j1Map::LoadMap()
{

	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");

		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.music = map.child("properties").child("property").attribute("value").as_string();
		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

//Load tileset data from a node ----------------------------------------------
bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

//Load an image for every tileset from a node ----------------------------------------------
bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");

		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

//Load layer data from a node ----------------------------------------------
bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	
		LoadProperties(node, layer->property);
		
	pugi::xml_node layer_data = node.child("data");

	if(layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");

		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width*layer->height];
		memset(layer->data, 0, layer->width*layer->height);

		int i = 0;
		for(pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

// Load a group of properties from a node and fill an array with it ----------------------------------------------
bool j1Map::LoadProperties(pugi::xml_node& node,Properties property[])
{
	LOG("Loading properties");

	pugi::xml_node layer;
	bool ret = true;
	
	int i = 0;
	
	for (layer = node.child("properties").child("property"); layer && ret; layer = layer.next_sibling("property"))
	{

		property[i].name = layer.attribute("name").as_string();
		if (property[i].name == "Nodraw") {
			property[i].prop.ivalue = layer.attribute("value").as_int();
			i++;
		}
	
		
			if (property[i].name == "Navigation") {
				property[i].prop.ivalue = layer.attribute("value").as_int();
				i++;
			}

			if (property[i].name == "Parallax") {
				property[i].prop.fvalue = layer.attribute("value").as_float();
				i++;
			}
			
	}

	return ret;
}

int MapLayer::returnPropValue(const char* propName) {
	int value = -1;
	for (int i = 0; i < MAX_PROPERTIES; i++){

		if (property[i].name == propName) {
			value = property[i].prop.ivalue;
		}
	}
	
	return value;
}

float MapLayer::returnPropfValue(const char* propName) {
	float value = -1;
	for (int i = 0; i < MAX_PROPERTIES; i++) {

		if (property[i].name == propName) {
			value = property[i].prop.fvalue;
		}
	}

	return value;
}
bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer)
{
	bool ret = false;
	
	list<MapLayer*>::iterator Layer_list;
	MapLayer* Layer_it;


	for (Layer_list = data.layers.begin(); Layer_list != data.layers.end(); ++Layer_list)
	{
		MapLayer* layer = *Layer_list;
		LOG("%s %d", layer->name.GetString(), layer->returnPropValue("Navigation"));
		if (layer->returnPropValue("Navigation") != 1)
			continue;

		uchar* map = new uchar[layer->width * layer->height];
		memset(map, 1, layer->width * layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y * layer->width) + x;

				int tile_id = layer->Get(x, y);
				if (tile_id != 0)
				{
					map[i] = 0;

					/*TileType* ts = tileset->GetTileType(tile_id);
					if(ts != NULL)
					{
						map[i] = ts->properties.Get("walkable", 1);
					}*/
				}
				LOG("%d", map[i]);

			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}
