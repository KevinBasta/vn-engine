

/*
* for each version of texture store picker in engine node action fields.cpp
* The one advantage over an iterator is that you don't have to init the iterator
* or check the iterator bounds in the container or increment the iterator which is nice
* 
* 
std::for_each(
	textureStores.begin(),
	textureStores.end(),
	[&modified, &texture](auto& entry) {
		const bool isSelected{ entry.first == texture.m_textureStoreId };
		if (ImGui::Selectable(entry.second.get()->getName().c_str(), isSelected)) {
			texture.m_textureStoreId = entry.first;
			modified = true;
		}

		if (isSelected) {
			ImGui::SetItemDefaultFocus();
		}
	}
);

*/