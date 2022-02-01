#ifndef IMAGE_H
#define IMAGE_H

#include <cstdint>
#include <vector>
#include <filesystem>

struct colour {
	uint16_t channel[4];
};
enum channelNames { red, green, blue, alpha };


// a utility function, similar to atoi
// two main issues are present:
// - it does not properly check for whitespace, only that the char is not a number
// - it does not handle extra whitespace before or after a value
int pullIntFromFilebuf(std::filebuf* buf){
	bool grabChars = true;
	int data = 0;
	char tmp;
	while(grabChars){
		tmp = buf->sbumpc();
		if(tmp < 0x30 || tmp > 0x39){
			grabChars = false;
			break;	
		} else {
			tmp -= 0x30; //convert from text to number
		}
		data *=10;	
		data += tmp;
	}
	return data;
}
// 'image' class definintion
class image {
public:
	image() = default;

	std::vector<colour> frameBuffer;
	int getWidth();
	int getHeight();
	int getBitDepth();
	void setWidth(int input);
	void setHeight(int input);
	void setBitDepth(int input);

//private:
	int width, height, bitDepth;

	colour getPixelColour(int x, int y);
	void setPixelColour(int x, int y, colour c);
};

// 'image' public member functions
int image::getWidth(){
	return width;
}

int image::getHeight(){
	return height;
}

int image::getBitDepth(){
	return bitDepth;
}

void image::setWidth(int input){
	width = input;
} 
void image::setHeight(int input){
	height = input;
} 
void image::setBitDepth(int input){
	bitDepth = input;
} 

// 'image' private member functions
colour image::getPixelColour(int x, int y){
	// check for invalid inputs
	bool validCoordinates = (x >= 0 && x < width && y >= 0 && y < height);
	std::cerr << "Color validation has not been implemented!\n";
	if(validCoordinates){
		return frameBuffer[y*width + x];	
	} else {
		std::cerr << "Bad coordinates!\n";
		return {0, 0, 0, 0};
	}
}

void image::setPixelColour(int x, int y, colour c){
	// check for invalid inputs
	bool validCoordinates = (x >= 0 && x < width && y >= 0 && y < height);
	std::cerr << "Color validation has not been implemented!\n";
	if(validCoordinates){
		frameBuffer[y*width + x] = c;	
	} else {
		std::cerr << "Bad coordinates!\n";
	}
}
// end 'image' definition and member implementations

// begin 'imageFormat' class definition
class imageFormat {
public:
	imageFormat() = default;
	//imageFormat(image input);

	image img;
	virtual int encode();
	virtual int decode(std::filebuf* b);
};

// 'imageFormat' public member functions
//imageFormat::imageFormat(image input) : img(input){}

int imageFormat::encode(){
	return 1;
}

int imageFormat::decode(std::filebuf* buffer){
	return 1;
}
// end 'imageFormat' definition and member implementations

// begin ppm fileformat handler
/*
	The ppm format is rather simple. It consists of a text file, with all colour data in ascii numeric form. Each unique piece of information is seperated by whitespace.

	The header of the ppm format lists all information in latin character ASCII. Numbers are written in decimal base.
	The first line of the header indicates the file type. The option 'P6' indicates the 'raw' ppm format, and 'P3' indicates 'plain' format.:
	P3
	The next line lists the x and y dimensions in pixels:
	1920 1080
	The final line of the heading information is the bit depth, indicated by what the maximum value is. This can be from 65536 to 1:
	255	

	Following this is each pixels data, listed by each row top to bottomw, then within each row left to right. The data lists red, then green, then blue data. The 'raw' format uses 1 byte (or 2 if sample depth exceeds 255) per colour. The 'plain' format uses an ascii representation, using the human readable latin character set (this means the value 255 would use 3 bytes).
*/
class ifPPM : public imageFormat {
public:
//	ifPPM() = default;

	int encode() override;
	int decode(std::filebuf* b) override;

}; 

int ifPPM::encode(){
	return 1;
}

int ifPPM::decode(std::filebuf* dataBuffer){
	enum dataFormat {
		raw1Byte,
		raw2Byte,
		plain
	};
	// read header for raw/plain version
	dataBuffer->sbumpc();//consume the P from header
	int version = pullIntFromFilebuf(dataBuffer);
	// read header for dimensions
	img.setWidth(pullIntFromFilebuf(dataBuffer));
	img.setHeight(pullIntFromFilebuf(dataBuffer));
	// read header to find bit depth
	uint16_t tmpBD = 0;
	uint16_t maxVal = pullIntFromFilebuf(dataBuffer);	
	while((1 << tmpBD) <= maxVal){
		tmpBD++;
	}
	img.setBitDepth(tmpBD);
	dataFormat dataMode;
	if(version == 3){
		dataMode = plain;
	} else { //P6
		if(img.getBitDepth() <= 8){
			dataMode = raw1Byte;
		} else {
			//multibyte data
			dataMode = raw2Byte;	
		}
	}

	std::cout << "Image width: " << img.getWidth() << "\n";
	std::cout << "Image height: " << img.getHeight() << "\n";
	std::cout << "Image bit depth: " << img.getBitDepth() << "\n";
	img.frameBuffer.reserve(img.getWidth() * img.getHeight());
	// iterate through pixel data
	for(int y = 0; y < img.getHeight(); y++){
	for(int x = 0; x < img.getWidth(); x++){
		colour col;
		for(int ch = 0; ch < 3; ch++){
			uint16_t data, tmp;
			switch(dataMode){
			case raw1Byte:
				//fetch 1 byte
				data = dataBuffer->sbumpc();	
				break;
			case raw2Byte:
				//fetch 2 bytes
				tmp = dataBuffer->sbumpc();
				data = (tmp << 8) + dataBuffer->sbumpc();	
				break;
			case plain:
				data = pullIntFromFilebuf(dataBuffer);
				break;
			} // end switch
			col.channel[ch] = data;
		} // end channels loop
		col.channel[3] = 0xFFFF;
		img.setPixelColour(x, y, col);	
	} // end x
	} // end y
		
	for(int p =0;p < img.getWidth()*img.getHeight();p++){
		for(int ch =0;ch < 4;ch++){
			std::cout << "p: " << p << " ch: " << ch << " val: " << img.frameBuffer[p].channel[ch] << "\n";
		}
	};
	return 1;
}
#endif // IMAGE_H
