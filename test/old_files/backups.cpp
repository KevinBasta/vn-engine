
// Set path of test files
std::filesystem::path jsonpath = "C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\tests\\Json\\";
//std::filesystem::path filepath = std::filesystem::absolute(jsonpath);

std::fstream newfile;

std::string fil = "C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\file.txt";

newfile.open((jsonpath.string()).append("test1.json"), std::ios::in);

if (!newfile.is_open()) {
	std::cout << "open failed" << '\n';
}
else if (newfile.is_open()) {
	std::cout << "read start" << '\n';
	std::string tp;

	while (getline(newfile, tp)) {
		std::cout << tp << '\n';
	}
}
//std::cout << filepath << std::endl;

//JsonLexer *test = new JsonLexer(filepath);
