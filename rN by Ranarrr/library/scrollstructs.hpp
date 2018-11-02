#ifndef SCROLLSTRUCTS_HPP
#define SCROLLSTRUCTS_HPP

#pragma once

#include <vector>

class scrollframestruct {
	float m_flGroundHeight;
	bool didscroll;

public:
	scrollframestruct( float gh, bool scroll ) {
		m_flGroundHeight = gh;
		didscroll = scroll;
	}

	float getgh() {
		return m_flGroundHeight;
	}

	bool didframescroll() {
		return didscroll;
	}
};

class scrollpattern { // lets do avg of last scrolled and next frame GH for end, same for start just the prev frame.
	std::vector<scrollframestruct> didscrollframe;

public:
	scrollpattern() {}

	scrollpattern( std::vector<scrollframestruct> frames ) {
		didscrollframe = frames;
	}

	std::vector<scrollframestruct> getscrollframes() {
		return didscrollframe;
	}

	float getStart() {
		if( didscrollframe.size() > 0 )
			return didscrollframe.at( 0 ).getgh();
	}

	float getEnd() {
		if( didscrollframe.size() > 0 )
			return didscrollframe.at( didscrollframe.size() - 1 ).getgh();
	}

	int getTickLength() {
		return didscrollframe.size();
	}
};

#endif // !SCROLLSTRUCTS_HPP
