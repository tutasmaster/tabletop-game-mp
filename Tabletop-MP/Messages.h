enum class MESSAGE_TYPE : unsigned char {
	UPDATE_TABLE = 0, //SERVER = CLIENT 
	UPDATE_ENTITY = 1 //SERVER = CLIENT
};


//MESSAGE_TYPE + ID + MESSAGE_TABLE
enum class MESSAGE_TABLE : unsigned char {
	INSERT = 0,  //STRING NAME + FLOAT WIDTH + FLOAT HEIGHT
	REMOVE = 1, 
	ID = 2,      //UNSIGNED INTEGER ID
	NAME = 3,    //STRING NAME
	SIZE = 4    //FLOAT WIDTH + FLOAT HEIGHT
};

//MESSAGE_TYPE + TABLE_ID + ENTITY_ID + MESSAGE_ENTITY
enum class MESSAGE_ENTITY : unsigned char {
	INSERT = 0,         //TODO
	REMOVE = 1,	        
	OWNER = 1,	        //UNSIGNED INTEGER PLAYER_ID
	ID = 2,		        //UNSIGNED INTEGER ID
	POS = 3,	        //FLOAT X + FLOAT Y
	MOVABLE = 4,        //BOOL IS_MOVABLE
	ASSET_ID = 5,       //UNSIGNED INTEGER ASSET_ID
	FLAGS = 6,	        //UNSIGNED CHAR FLAGS
	FLIP = 7,	        //UNSIGNED INTEGER ASSET_ID
	ROTATE = 8,         //FLOAT ROTATION
	POS_UNRELIABLE = 9, //FLOAT X + FLOAT Y
};