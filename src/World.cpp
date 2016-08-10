#include "World.h"
#include "math.h"


World::World(sf::RenderTarget & outputTarget, FontHolder& fonts) :
	target_(outputTarget),
	sceneTexture_(),
	worldView_(target_.getDefaultView()),
	textures_(),
	fonts_(fonts),
	sceneGraph_(),
	sceneLayers_(),
	commandQueue_(),
	worldBounds_(0.f, 0.f, worldView_.getSize().x, worldView_.getSize().y),
	spawnPosition_(worldView_.getSize().x / 2.f, worldBounds_.height - worldView_.getSize().y / 2.f),
	playerCharacter_(nullptr),
	maze_(nullptr)
{
	sceneTexture_.create(target_.getSize().x, target_.getSize().y);

	loadTextures();
	buildScene();

	worldView_.setCenter(spawnPosition_);
}

void World::update(sf::Time dt)
{
	//worldView_.move(0.f, scrollSpeed_ * dt.asSeconds());
	playerCharacter_->velocity(0.f, 0.f);

	while (!commandQueue_.isEmpty())
		sceneGraph_.onCommand(commandQueue_.pop(), dt);
	adaptPlayerVelocity();

	//handleCollisions();

	sceneGraph_.update(dt, commandQueue_);
}

void World::draw()
{
	//target_.setView(worldView_);
	target_.draw(sceneGraph_);
}

CommandQueue & World::commandQueue()
{
	return commandQueue_;
}

bool World::hasPlayerReachEnd() const
{
	return !worldBounds_.contains(playerCharacter_->getPosition());
}

void World::loadTextures()
{
	textures_.load(TextureId::Tiles, "resources/png/spritesheet_tiles.png");
	textures_.load(TextureId::Characters, "resources/png/spritesheet_characters.png");
}

void World::buildScene()
{
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		CategoryType category = CategoryType::SceneLayer;

		SceneNode::uPtr layer(new SceneNode(category));
		sceneLayers_[i] = layer.get();

		sceneGraph_.attachChild(std::move(layer));
	}

	//Build world here
	std::unique_ptr<Maze> maze(new Maze(textures_));
	maze_ = maze.get();
	sceneLayers_[Background]->attachChild(std::move(maze));

	std::unique_ptr<Character> player(new Character(Character::Woman, textures_));
	playerCharacter_ = player.get();
	playerCharacter_->setPosition(spawnPosition_);
	sceneLayers_[Foreground]->attachChild(std::move(player));
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	sceneGraph_.checkSceneCollision(sceneGraph_, collisionPairs);

	for (SceneNode::Pair pair : collisionPairs)
	{
		if (matchCategories(pair, CategoryType::PlayerCharacter, CategoryType::Walls))
		{
			auto& player = static_cast<Character&>(*pair.first);

			//player.stepBack(); TODO
		}
	}
}

void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = playerCharacter_->velocity();

	if (velocity.x != 0.f && velocity.y != 0.f)
		playerCharacter_->velocity(velocity / std::sqrt(2.f));
}

bool World::matchCategories(SceneNode::Pair & colliders, CategoryType type1, CategoryType type2)
{
	CategoryType cat1 = colliders.first->category();
	CategoryType cat2 = colliders.second->category();

	if ((type1 & cat1) != CategoryType::None && (type2 & cat2) != CategoryType::None)
	{
		return true;
	}
	else if ((type1 & cat2) != CategoryType::None && (type2 & cat1) != CategoryType::None)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

sf::FloatRect World::viewBounds() const
{
	return sf::FloatRect(worldView_.getCenter() - worldView_.getSize() / 2.f, worldView_.getSize());
}