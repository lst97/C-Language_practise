#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define 逃犯條例修訂草案 Bill_2019
#define 該國家或地區和香港沒有長期引渡協議 ExtraditionAgreement
#define 疑犯 theSuspect
#define 特別移交安排 fnSpecialHandoverArrangement
#define 最高人民檢察院提出涉及的罪行 1
#define 可判處最高刑期七年或以上 1
#define 要求 theClaim
#define 行政長官審視 fnExamineTheCase
#define 對涉案人士發出拘捕令 fnIssueAuthorization
#define 裁判法院審理案件 fnTrialCase
#define 表面證據足夠 evidence
#define 沒有政治因素  noPoliticalFactor
#define 發出拘押令 fnDetained
#define 符合 1
#define 提出人身保護令 personalProtection
#define 重新審視個案 fnRevisitCase
#define 維持原判 0
#define 特首考慮疑犯人權 fnConsiderHumanRights
#define 發出最後移交命令 fnFinalHandover

int 特別移交安排(char );
bool 行政長官審視(int );
void 對涉案人士發出拘捕令(void);
bool 裁判法院審理案件(char );
void 發出拘押令(char );
bool 重新審視個案(char);
void 特首考慮疑犯人權(void);
void 發出最後移交命令(void);

int main(void) {
	char* 疑犯 = 0; // Suspect found.

	bool 逃犯條例修訂草案 = false;
	bool 該國家或地區和香港沒有長期引渡協議 = false;

	// 立法會將於6月12日就修例恢復二讀辯論，盛傳會在6月27日表決。
	if (逃犯條例修訂草案 == true) {
		if (該國家或地區和香港沒有長期引渡協議 == true) {
			特別移交安排(疑犯);
		}
	}
	
	printf("反送中、撒惡法!");
	system("pause");
	return 0;
}

int 特別移交安排(char 疑犯){
	int 要求;
	bool 提出人身保護令 = 0;

	要求 = 最高人民檢察院提出涉及的罪行 + 可判處最高刑期七年或以上;
	if (行政長官審視(要求) == 符合) {
		對涉案人士發出拘捕令();
		裁判法院審理案件(疑犯);
		while (裁判法院審理案件(疑犯) == 符合) {
			if (提出人身保護令 == true) {
				if (重新審視個案(疑犯) == 維持原判) {
					特首考慮疑犯人權();
					發出最後移交命令();
					// 可否申請回港服刑，港府會與內地再跟進.
					// 如果移交地的政府侵害疑犯人權，特區政府有何跟進方法? 保安局局長李家超只表示會進行「交涉」。
				}
				else break;
			}
		}
	}
}

bool 行政長官審視(int 要求) {
	// I don't know how this works. somehow return 符合.
	return 符合;
}
bool 裁判法院審理案件(char 疑犯) {
	bool 表面證據足夠 = 0, 沒有政治因素 = 0;
	if (表面證據足夠 == true && 沒有政治因素 == true) {
		發出拘押令(疑犯);
		return 符合;
	}
}
void 特首考慮疑犯人權(void) {
	// I don't know how this works either
}

void 對涉案人士發出拘捕令(void) {

}

void 發出拘押令(char 疑犯) {

}
bool 重新審視個案(char 疑犯) {
	return 維持原判;
}
void 發出最後移交命令(void) {

}
