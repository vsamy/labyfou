#pragma once

#include <memory>
#include <vector>
#include <utility>
#include <set>
#include <algorithm>
#include <functional>
#include <SFML/Graphics.hpp>
#include "Command.h"


class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr<SceneNode> uPtr;
	typedef std::pair<SceneNode*, SceneNode*> Pair;

public:
	explicit SceneNode(CategoryType category = CategoryType::None);
	~SceneNode();

	void attachChild(uPtr child);
	uPtr detachChild(const SceneNode& node);

	void update(sf::Time dt, CommandQueue& commands);

	sf::Transform worldTransform() const;
	sf::Vector2f worldPosition() const;

	void onCommand(const Command& command, sf::Time dt);
	virtual CategoryType category() const;

	virtual void checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs);
	void checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs);
	void removeWrecks();
	virtual sf::FloatRect hitBox() const;
	virtual bool isMarkedForRemoval() const;
	virtual bool isDestroyed() const;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void drawHitBox(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	void updateChildren(sf::Time dt, CommandQueue& commands);

protected:
	std::vector<sf::FloatRect> hitBox_;
	std::vector<uPtr> children_;
	
private:
	SceneNode* parent_;
	CategoryType defaultCategory_;
};