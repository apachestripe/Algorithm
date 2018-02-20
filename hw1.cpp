#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <sys/resource.h>
using namespace std;
/****  Your homework starts here ****/
bool isLegal(string firstWord, string secondWord){
	int size = firstWord.length() - secondWord.length();
	if(size > 1 || size < -1 || firstWord == secondWord)
		return false;
	int index = 0;
	while(firstWord[index] == secondWord[index])
		index++;
	if(size <= 0)
		index++;
	while(index < firstWord.size() - size){
		if(firstWord[index + size] != secondWord[index])
			return false;
		index ++;
	}
	return true;
}
vector< vector<string> > findLadders(string beginWord, string endWord, vector<string>& wordDictionary){
	vector< vector<string> > ans;
	vector<string> storeDictionary;
	int index=0;
	while(index < wordDictionary.size()){
		for(int repeatIndex = index + 1 ; repeatIndex < wordDictionary.size() ; repeatIndex ++)
			if(wordDictionary[index] == wordDictionary[repeatIndex])
				wordDictionary.erase(wordDictionary.begin() + repeatIndex);
		if(isLegal(beginWord,wordDictionary[index])){
    			vector<string> newLadder;
    			newLadder.push_back(beginWord);
    			newLadder.push_back(wordDictionary[index]);
    			ans.push_back(newLadder);
			storeDictionary.push_back(wordDictionary[index]);
			wordDictionary.erase(wordDictionary.begin() + index);
		}
		index++;
	}
	int stopIndex = wordDictionary.size() - 1;
	index = 0;
	vector<int> checkedLadder(wordDictionary.size() + 1, 0);
	int minLength = 2147483647;
	while(index != stopIndex){
		bool findLadder = false;
		int ladderNumber = checkedLadder[index + 1];
		while(ladderNumber < ans.size()){
			if(ans[ladderNumber].size() < minLength - 1 && isLegal(ans[ladderNumber].back(), wordDictionary[index])){
				vector<string> newLadder;
	    			for(int copyIndex = 0 ; copyIndex < ans[ladderNumber].size() ; copyIndex++)
	    				newLadder.push_back(ans[ladderNumber][copyIndex]);
	    			newLadder.push_back(wordDictionary[index]);
	    			ans.push_back(newLadder);
	    			findLadder=true;
			}
			ladderNumber++;
		}
		ladderNumber = checkedLadder[0];
		while(ladderNumber < ans.size()){
			if(ans[ladderNumber].size() > minLength || (ans[ladderNumber].size() == minLength && !isLegal(ans[ladderNumber].back(),endWord))){
				ans.erase(ans.begin() + ladderNumber);
				continue;
			}
			else if(isLegal(ans[ladderNumber].back(), endWord))
				minLength = ans[ladderNumber].size();
			ladderNumber++;
		}
		if(findLadder){
		  	storeDictionary.push_back(wordDictionary[index]);
	 		wordDictionary.erase(wordDictionary.begin() + index);
			checkedLadder.erase(checkedLadder.begin() + index + 1);
	    		stopIndex = index == 0 ? wordDictionary.size() - 1 : index - 1;
	    		//cout<<ans.size()<<" "<<worddictionary.size()<<endl;
	        }
	    	else
			checkedLadder[index + 1] = ans.size();
		index = index < wordDictionary.size() - 1 ? index + 1 : 0;
		checkedLadder[0] = ans.size();
    	}
    	/*
    	cout<<minlength<<endl;
    	for(int i=0;i<ans.size();i++){
    		for(int j=0;j<ans[i].size();j++)
    	       		cout<<ans[i][j]<<" ";
    		cout<<endl;
    	}
    	*/
	int ladderNumber = 0 ;
	while(ladderNumber < ans.size()){
		if(!isLegal(ans[ladderNumber].back(), endWord))
			ans.erase(ans.begin() + ladderNumber);
		else{
			ans[ladderNumber].push_back(endWord);
			ladderNumber ++;
		} 
	}
	while(!storeDictionary.empty()){
		wordDictionary.push_back(storeDictionary.back());
		storeDictionary.pop_back();
	}
	// return an empty vector if you cannot find one
	return ans;
}
/****  your homework ends here ****/
int main(int argc, char* argv[]){
	/* input check */
	if (argc < 3){
		cout << "usage: " << argv[0] << " <input_file> <output_file>" << endl;
		return -1;
	}
	/* file check */
	ifstream ifs(argv[1]);
	if (!ifs.is_open()){
		cout << "input file: " << argv[1] << " cannot be opened." << endl;
		return -1;
	}
	ofstream ofs(argv[2]);
	if (!ofs.is_open()){
		cout << "output file: " << argv[2] << " cannot be opened." << endl;
		return -1;
	}
	/* input data from the file */
	string beginword, endword;
	vector<string> worddictionary;
	ifs >> beginword >> endword;
	while (!ifs.eof()){
		string tmp_str;
		ifs >> tmp_str;
		worddictionary.push_back(tmp_str);
	}
	ifs.close();	
	/* function */
	vector< vector<string> > answer;	
	const clock_t start_time = clock();
	answer = findLadders(beginword, endword, worddictionary);
	double run_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;	
	/* simply examine the answer*/
	if (answer.size() > 0){
		// 1. check if the begin and end words are in the answer
		for (int i = 0; i < answer.size(); i++){
			if ((answer[i])[0].compare(beginword) != 0){
				cout << "error: the word (" << (answer[i])[0] << ") is not the begin word." << endl;
				return -1;
			}	
			else if ((answer[i])[(answer[i]).size()-1].compare(endword) != 0){
				cout << "error: the word (" << (answer[i])[(answer[i]).size()-1] << ") is not the end word." << endl;
				return -1;
			}
		}
		// 2. check if the answer strings have the same length
		if (answer.size() > 1){
			int length = (answer[0]).size();
			for (int i = 1; i < answer.size(); i++){
				if (length != (answer[i]).size()){
					cout << "error: found different string lengths in the answer vector ([0] and [" << i << "])" << endl;
					return -1;
				}
			}
		}
		// 3. check if the answer is not in the dictionary
		for (int i = 0; i < answer.size(); i++){
			for (int j = 1; j < (answer[i]).size(); j++){
				bool is_indic = false;
				for (int k = 0; k < worddictionary.size(); k++){
					if ((answer[i])[j].compare(worddictionary[k]) == 0){
						is_indic = true;
						break;
					}
				}
				if (!is_indic){
					cout << "error: the word (" << (answer[i])[j] << ") is not in the dictionary." << endl;
					return -1;
				}
			}
		}
		// 4. check repeated
		for (int i = 0; i < answer.size(); i++){
			for (int j = 0; j < (answer[i]).size(); j++){
				bool is_repeated = false;
				for (int k = j-1; k >= 0; k--){
					if ((answer[i])[j].compare((answer[i])[k]) == 0){
						is_repeated = true;
						break;
					}
				}
				if (is_repeated){
					cout << "error: the word (" << (answer[i])[j] << ") repeated in the answer[" << i << "]." << endl;
					return -1;
				}
			}
		}
		// 5. check legal transform
		for (int i = 0; i < answer.size(); i++){
			for (int j = 0; j < (answer[i]).size()-1; j++){
				short len_diff = ((answer[i])[j]).length() - ((answer[i])[j+1]).length();
				if (len_diff > 1 || len_diff < -1){
					cout << "error: the word (" << ((answer[i])[j]) << ") and (" << ((answer[i])[j+1]) << ") has invalid length difference."  << endl;
					return -1;
				}
				else if (len_diff == 0){
					short char_diff = 0;
					for (int idx = 0; idx < ((answer[i])[j]).length(); idx++){
						if (((answer[i])[j])[idx] != ((answer[i])[j+1])[idx]){
							char_diff++;
						}
					}
					if (char_diff != 1){
						cout << "error: the word (" << ((answer[i])[j]) << ") and (" << ((answer[i])[j+1]) << ") has invalid transformation."  << endl;
						return -1;
					}
				}
				else if (len_diff == 1){
					int str1_idx = 0;
					short char_diff = 0;
					for (int idx = 0; idx < ((answer[i])[j+1]).length(); idx++){
						if (((answer[i])[j])[str1_idx] != ((answer[i])[j+1])[idx]){
							char_diff++;
							idx--;
						}
						str1_idx++;
					}	
					if (char_diff > 1){
						cout << "error: the word (" << ((answer[i])[j]) << ") and (" << ((answer[i])[j+1]) << ") has invalid transformation."  << endl;
						return -1;
					}
				}
				else if (len_diff == -1){
					int str2_idx = 0;
					short char_diff = 0;
					for (int idx = 0; idx < ((answer[i])[j]).length(); idx++){
						if (((answer[i])[j])[idx] != ((answer[i])[j+1])[str2_idx]){
							char_diff++;
							idx--;
						}
						str2_idx++;
						if (char_diff > 1){
							cout << "error: the word (" << ((answer[i])[j]) << ") and (" << ((answer[i])[j+1]) << ") has invalid transformation."  << endl;
							return -1;
						}
					}
				}
			}
		}
	}
	cout << "your result is valid." << endl;
	struct rusage r_usage;
	getrusage(RUSAGE_SELF,&r_usage);
	printf("Memory usage: %ld k-bytes\n",r_usage.ru_maxrss);
	cout << "=====  answer report  =====" << endl;
	cout << "CPU run time: " << run_time << endl;
	cout << "Mem Usage: " << r_usage.ru_maxrss << endl;
	cout << "Word ladder number: " << answer.size() << endl;
	if (answer.size() > 0){
		cout << "word ladder length: " << (answer[0]).size() << endl;
		for (int i = 0; i < answer.size(); i++){
			cout << "ans[" << i << "]: ";
			for (int j = 0; j < (answer[i]).size()-1; j++){
				cout << (answer[i])[j] << " -> ";
			}
			cout << (answer[i])[(answer[i]).size()-1] << endl;
		}
	} 
	ofs << "=====  answer report  =====" << endl;
	ofs << "CPU run time: " << run_time << endl;
	ofs << "Mem Usage: " << r_usage.ru_maxrss << endl;
	ofs << "Word ladder number: " << answer.size() << endl;
	if (answer.size() > 0){
		ofs << "word ladder length: " << (answer[0]).size() << endl;
		for (int i = 0; i < answer.size(); i++){
			ofs << "ans[" << i << "]: ";
			for (int j = 0; j < (answer[i]).size()-1; j++){
				ofs << (answer[i])[j] << " -> ";
			}
			ofs << (answer[i])[(answer[i]).size()-1] << endl;
		}
	}
	ofs.close();
	return 0;
}
