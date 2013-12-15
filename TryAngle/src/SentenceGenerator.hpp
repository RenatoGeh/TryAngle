/*
 * SentenceGenerator.hpp
 *
 *  Created on: Dec 14, 2013
 *      Author: Renato Lui Geh
 */

#ifndef SENTENCEGENERATOR_HPP_
#define SENTENCEGENERATOR_HPP_

class SentenceGenerator {
	public:
		unsigned long int seed;

		/* Defines the size of the sentence.
		 * 	Let:
		 * 		Noun = N
		 * 		Direct Pronoun = P
		 * 		Possessive Pronoun = PP
		 * 		Verb = V
		 * 		Adjective = ADJ
		 * 		Adverb = ADV
		 * 		n = number [1, 3]
		 *
		 *	0 = 1 N
		 *	1 = 1 P/N + 1 V + 1 P/N
		 *	2 = 1 P/N + 1 ADV + 1 V + 1 P/N
		 *	3 = 1 ADJ + 1 N + 1 ADV + 1 V + 1 ADJ + 1 N
		 *	4 = [n] ADJ + 1 N + [n] ADV + [n] V + [n] ADJ + 1 N
		 *
		 *	In all cases we add a random punctuation mark at the end.
		 *
		 */
		unsigned char size;

		std::default_random_engine gen;
	public:
		SentenceGenerator(unsigned char, unsigned long int);
		~SentenceGenerator(void);
	public:
		std::string get(void);
	public:
		void setSeed(unsigned long int);
		void setSize(unsigned char);
	public:
		unsigned long int getSeed(void) const;
		unsigned char getSize(void) const;
	private:
		std::string getItem(const std::string[]);
		std::string getItems(const std::string[]);
	private:
		static int sizes[];

		static const std::string nouns[];
		static const std::string verbs[];
		static const std::string adverbs[];
		static const std::string adjectives[];
		static const std::string pronouns_direct[];
		static const std::string pronouns_possess[];
		static const std::string punctuations[];

		static const std::string EndOfFile;
	public:
		static std::string generate(void);
	private:
		static int arraySize(const std::string[]);
		inline static int arrayIndex(const std::string[]);
};

const std::string SentenceGenerator::EndOfFile("<EOF>");

const std::string SentenceGenerator::nouns[] = {"Shoe", "Lace", "Keyboard",
	"Crystal", "Ham", "Sandwich", "Jesus", "Canoe", "Ben", "Bean", "Head",
	"Machine", "Man", "Woman", "Bottle", "Rum", "Beer", "Wine", "Lasagna",
	"Mind", "Cabbage", "Body", "Band", "Pepper", "Salt", "Harry", "Sandra",
	"John", "Paul", "Benny", "Ball", "Grace", "Skyler", "Bryan", "Tom",
	"Hammer", "Turtle", "Duck", "Money", "Bridge", "Card", "Potato",
	"Elbow", "Watch", "Skin", "Toe", "Hand", "Foot", "Lettuce", "Floor",
	"Elephant", "Pizza", "Bus", "Sand", "Camel", "Room", "Pajamas", "Socks",
	"Paper", "Nose", "Slime", "Boot", "Star", "Engine", "Pillow", "Shadow",
	"Nail", "Kidney", "Package", "Tree", "Dress", "Planet", "Rocket", "River",
	"Fridge", "Engine", "Flowers", "Table", "Mouse", "Leg", "Stomach", "Toe",
	"TV", "Train", "Bear", "Beard", "Cupcake", "Cake", "Paper", "Wallet",
	EndOfFile
};
const std::string SentenceGenerator::verbs[] = {"Did", "Does", "Made",
	"Killed", "Saved", "Ate", "Played with", "Went to", "Canned",
	"Told", "Chewed", "Poured", "Initiated", "Challenged", "Grabbed",
	"Groped", "Stopped", "Nursed", "Ordered", "Oppressed", "Hurt",
	"Insulted", "Joined", "Forgave", "Denied", "Attacked", "Defended",
	"Enthralled", "Kissed", "Hugged", "Touched", "Massaged", "Nailed",
	"Hit", "Empowered", "Crushed", "Corrupted", "Worshiped", "Wooed",
	"Weakened", "Undressed", "Tormented", "Tickled", "Tested", "Terrorized",
	"Threatened", "Revived", "Held", "Electrified", "Entertained", "Enticed",
	"Performed", "Placed", "Played", "Predicted", "Printed", "Proposed", "Ran",
	EndOfFile
};
const std::string SentenceGenerator::adverbs[] = {"Accidentally", "Angrily",
	"Anxiously", "Awkwardly", "Blindly", "Boldly", "Bravely", "Crazily",
	"Coyly", "Defiantly", "Deftly", "Doubtfully", "Eagerly", "Elegantly",
	"Foolishly", "Happily", "Hastily", "Frantically", "Innocently", "Gleefuly",
	"Irritably", "Jealously", "Honestly", "Lazily", "Madly", "Merrily", 
	"Mysteriously", "Hungrily", "Nervously", "Obediently", "Politely",
	"Poorly", "Powerfully", "Promptly", "Quickly", "Rudely", "Selfishly",
	"Seriously", "Shakily", "Silently", "Slowly", "Solemnly", "Sternly",
	"Steadly", "Tenderly", "Tediously", "Terrifically", "Tightly", "Wildly",
	"Warmly", "Vivaciously", "Victoriously", "Unexpectedly", "Wearily",
	EndOfFile
};
const std::string SentenceGenerator::adjectives[] = {"Interesting",
	"Incredible", "Disgusting", "Revolting", "Sexy", "Adorable", "Adventurous",
	"Aggressive", "Alert", "Attractive", "Average", "Beautiful", "Blue-eyed",
	"Gray-eyed", "Dark-eyed", "Bloody", "Blushing", "Bright", "Clean", "Clear",
	"Cloudy", "Colorful", "Crowded", "Cute", "Dark", "Drab", "Distinct", "Dull",
	"Elegant", "Excited", "Fancy", "Filthy", "Glamorous", "Gleaming", "Gorgeous",
	"Graceful", "Grotesque", "Handsome", "Light", "Long", "Magnificent", "Misty",
	"Motionless", "Muddy", "Old-fashioned", "Plain", "Poised", "Precious", 
	"Quaint", "Sparkling", "Ugly", "Strange", "Unusual", "Annoying", "Busy",
	"Clever", "Clumsy", "Crazy", "Curious", "Dead", "Expensive", "Famous", 
	"Fragile", "Frail", "Gifted", "Helpless", "Horrible", "Important",
	"Innocent", "Inquisitive", "Modern", "Mushy", "Outstanding", "Poor", "Rich",
	"Powerful", "Puzzled", "Shy", "Stupid", "Talented", "Tame", "Tender", "Tough",
	"Uninterested", "Wild", "Angry", "Annoyed", "Anxious", "Arrogant", "Ashamed",
	"Awful", "Bewildered", "Blue", "Green", "Bored", "Clumsy", "Confused", "Cruel",
	"Dangerous", "Defiant", "Disgusted", "Embarrassed", "Envious", "Evil", "Dizzy", 
	"Frantic", "Frightened", "Grumpy", "Hungry", "Hurt", "Jealous", "Lazy", 
	"Lonely", "Mysterious", "Nasty", "Naughty", "Nervous", "Scary", "Selfish",
	"Terrible", "Tired", "Upset", "Weary", "Worried", "Cautious", "Thoughtless",
	EndOfFile
};
const std::string SentenceGenerator::pronouns_direct[] = {"I", "You", "He", "She",
	"We", "Anybody", "Anyone", "Anything", "Both", "Everybody", "Everyone", 
	"Everything", "It", "Itself", "Yourself", "Herself", "Himself", "Theirselves",
	"Ourselves", "Myself", "Many", "Nobody", "Nothing", "None", "Other", "Others",
	"Several", "Some", "Somebody", "Something", "This", "Whatever", "Whichever",
	"Yourselves", "Whomever", "Whoever", "They", "Him", "Her", EndOfFile
};
const std::string SentenceGenerator::pronouns_possess[] = {"Mine", "Your", "His",
	"Her", "Someones", "Anybodys", "Everyones", "Their", "Our", "Your",
	EndOfFile
};
const std::string SentenceGenerator::punctuations[] = {"!", ".", "...",
	EndOfFile
};

int SentenceGenerator::sizes[] = {-1, -1, -1, -1, -1, -1, -1};

SentenceGenerator::SentenceGenerator(
		unsigned char _size = 2,
		unsigned long int _seed = Utility::Random::getUnsignedRandom()) :
		seed(_seed), size(_size), gen(seed) {}

SentenceGenerator::~SentenceGenerator(void) {}

void SentenceGenerator::setSeed(unsigned long int _seed) {seed = _seed;}
void SentenceGenerator::setSize(unsigned char _size) {size = _size;}

unsigned long int SentenceGenerator::getSeed(void) const {return seed;}
unsigned char SentenceGenerator::getSize(void) const {return size;}

inline int SentenceGenerator::arrayIndex(const std::string array[]) {
	return array==nouns?0:array==verbs?1:array==adverbs?2:
			array==adjectives?3:array==pronouns_direct?4:
			array==pronouns_possess?5:6;
}

int SentenceGenerator::arraySize(const std::string array[]) {
	int index = arrayIndex(array);

	if(sizes[index]==-1) {
		int i;
		for(i=0;array[i].compare(SentenceGenerator::EndOfFile);++i);
		sizes[index] = i;
	}

	return sizes[index];
}

std::string SentenceGenerator::getItem(const std::string array[]) {
	return array[gen()%arraySize(array)] + " ";
}

std::string SentenceGenerator::getItems(const std::string array[]) {
	int i, n = (gen()%3)+1;
	std::string res;

	std::cout << n << std::endl;

	for(i=0;i<n;++i) {
		if(i==n-1 && n)
			res += "and ";
		else if(n)
			res += ", ";

		res += getItem(array);
	}

	return res;
}

std::string SentenceGenerator::get(void) {
	std::string res;

	switch(size) {
		case 0:
			res = getItem(nouns);
		break;
		case 1:
			res = gen()%2?getItem(nouns):getItem(pronouns_direct);
			res += getItem(verbs);
			res += gen()%2?getItem(nouns):getItem(pronouns_direct);
		break;
		case 2:
			res = gen()%2?getItem(nouns):getItem(pronouns_direct);
			res += getItem(adverbs);
			res += getItem(verbs);
			res += gen()%2?getItem(nouns):getItem(pronouns_direct);
		break;
		case 3:
			res = getItem(adjectives);
			res += getItem(nouns);
			res += getItem(adverbs);
			res += getItem(verbs);
			res += getItem(adjectives);
			res += getItem(nouns);
		break;
		case 4:
			res = getItems(adjectives);
			res += getItem(nouns);
			res += getItems(adverbs);
			res += getItems(verbs);
			res += getItems(adjectives);
			res += getItem(nouns);
		break;
		default:

		break;
	}

	res += getItem(punctuations);

	return res;
}

#endif
