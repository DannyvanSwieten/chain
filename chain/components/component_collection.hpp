//
//  component_collection.h
//  chain
//
//  Created by Danny van Swieten on 30/03/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include "components.hpp"
#include "component_storage.hpp"

using component_collection_t = ComponentStorageCollection
<
	Transform,
	RigidBody, BoundingBox, SpringJoint,
	StaticMesh,
	Script
>;
