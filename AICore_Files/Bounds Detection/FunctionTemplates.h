/*****************************************************************
Program Name: FunctionTemplates.h
Name: Zach Fetters
Description: This file will be used to declare various function
			 templates to be used through the game
*****************************************************************/

template <class T, class conT>
void EnforceNonPenetration(const T& p_cEntity, const conT& p_cContainerOfEntities)
{

	//iterate through all entities to check for overlapping bounds
	for(typename conT::const_iterator curEntity = p_cContainerOfEntities.begin();curEntity != p_cContainerOfEntities.end();++curEntity)
	{

		//check to make sure we don't comapre an entity with itself
		if(*curEntity == p_cEntity) 
		{

			continue;

		}

		//determine the distance between 2 entities
		Vector2D vToEntity = p_cEntity->Pos() - (*curEntity)->Pos(); 
		double dDistApart = vToEntity.Length();

		//calculate by how much two entities overlap based on their radii and how far apart they are
		double dOverlapAmount = (*curEntity)->BRadius() + p_cEntity->BRadius() - dDistApart;

		//if the entities overlap move them apart
		if(dOverlapAmount >= 0) 
		{

			p_cEntity->SetPos(p_cEntity->Pos() + (vToEntity/dDistApart) * dOverlapAmount);

		}

	}

}