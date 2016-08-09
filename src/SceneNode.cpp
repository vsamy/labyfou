#include "SceneNode.h"
#include <cassert>
#include <algorithm>
#include "utils.h"
#include "mathFunctions.h"

SceneNode::SceneNode(CategoryType category) : 
	children_(), 
	parent_(nullptr), 
	defaultCategory_(category)
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::attachChild(uPtr child)
{
	child->parent_ = this;
	children_.push_back(std::move(child));
}

SceneNode::uPtr SceneNode::detachChild(const SceneNode & node)
{
	auto found = std::find_if(children_.begin(), children_.end(), [&](uPtr& p) -> bool { return p.get() == &node; });

	assert(found != children_.end());

	uPtr result = std::move(*found);
	result->parent_ = nullptr;
	children_.erase(found);
	return result;

}

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
	updateCurrent(dt, commands);
	updateChildren(dt, commands);
}

sf::Transform SceneNode::worldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;

	for (const SceneNode* node = this; node != nullptr; node = node->parent_)
		transform = node->getTransform() * transform;

	return transform;
}

sf::Vector2f SceneNode::worldPosition() const
{
	return worldTransform() * sf::Vector2f();
}

CategoryType SceneNode::category() const
{
	return defaultCategory_;
}

sf::FloatRect SceneNode::hitBox() const
{
	return sf::FloatRect();
}

void SceneNode::onCommand(const Command & command, sf::Time dt)
{
	if ((command.category & category()) != CategoryType::None)
		command.action(*this, dt);
	for (uPtr& child : children_)
		child->onCommand(command, dt);
}

void SceneNode::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	drawCurrent(target, states);
	drawChildren(target, states);

	//drawHitBox(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget & /* target */, sf::RenderStates /* states */) const
{
}

void SceneNode::drawChildren(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (const uPtr& child : children_)
		child->draw(target, states);
}

void SceneNode::drawHitbox(sf::RenderTarget & target, sf::RenderStates /* states */) const
{
	sf::FloatRect rect = hitBox();

	sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);

	target.draw(shape);
}

void SceneNode::updateCurrent(sf::Time /* dt */, CommandQueue& /* commands */)
{
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
	for (uPtr& child : children_)
		child->update(dt, commands);
}

bool SceneNode::isMarkedForRemoval() const
{
	return isDestroyed();
}

bool SceneNode::isDestroyed() const
{
	return false;
}

void SceneNode::checkNodeCollision(SceneNode & node, std::set<Pair>& collisionPairs)
{
	if (this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed())
		collisionPairs.insert(std::minmax(this, &node));

	for (uPtr& child : children_)
		child->checkNodeCollision(node, collisionPairs);
}

void SceneNode::checkSceneCollision(SceneNode & sceneGraph, std::set<Pair>& collisionPairs)
{
	checkNodeCollision(sceneGraph, collisionPairs);

	for (uPtr& child : sceneGraph.children_)
		checkSceneCollision(*child, collisionPairs);
}