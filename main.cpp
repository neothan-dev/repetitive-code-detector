#include "common.h"
#include "FileType/FileType.h"
#include "Utils/Model/SA.h"
#include "Utils/Model/ST.h"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void processBlockContent(const std::string& filename, const std::vector<std::pair<std::string, std::pair<int32_t, std::string> > > &lsi, const bool is_svn) {
    int32_t n = lsi.size();

    std::vector<std::string> lines;
    std::vector<int32_t> lineIdx;
    for (auto psi : lsi) {
        lines.push_back(psi.first);
        lineIdx.push_back(psi.second.first);
    }

    lines.insert(lines.begin(), "");
    lineIdx.insert(lineIdx.begin(), 0);

    std::vector<int32_t> allHs;
    for (const auto &str : lines) {
        allHs.push_back(getHash(str));
    }

    SA* psa = new SA(allHs);

    ST* pst = new ST(psa->sa);

    std::vector<std::pair<std::pii, int32_t> > ans;

    // int32_t l = 0, r = n / 2 + 1;
    // auto check = [&](int32_t mid) -> bool {
    //     bool ret = false;

    //     int32_t lstPos = -1;
    //     for (int32_t i = 1; i <= n + 1; i++) {
    //         if (i <= n && psa->height[i] >= mid) {
    //             if (lstPos == -1) {
    //                 lstPos = i - 1;
    //             }
    //         } else {
    //             if (lstPos != -1) {
    //                 int32_t mnIdx = pst->queryMin(lstPos, i - 1);
    //                 int32_t mxIdx = pst->queryMax(lstPos, i - 1);
    //                 if (mxIdx - mnIdx >= mid) {
    //                     if (ans.empty() || ans.back().second <= mid) {
    //                         if (ans.back().second < mid) {
    //                             ans.clear();
    //                         }
    //                         ans.push_back({std::pii(mnIdx, mxIdx), mid});
    //                     }

    //                     ret = true;
    //                 }
    //             }
    //             lstPos = -1;
    //         }
    //     }

    //     return ret;
    // };
    // while (l + 1 < r) {
    //     int32_t mid = l + r >> 1;
    //     if (check(mid)) l = mid;
    //     else r = mid;
    // }

    for (int32_t i = 1; i <= n;) {
        int32_t mxLen = 0;
        int32_t rk = psa->rk[i];
        int32_t pre;

        pre = INF;
        for (int32_t j = rk - 1; j > 0; j--) {
            pre = min(pre, psa->height[j + 1]);
            if (pre == 0) break;
            int32_t idx = psa->sa[j];
            if (idx < i) continue;
            if (idx - i <= pre) {
                mxLen = max(mxLen, idx - i);
            }
        }

        pre = INF;
        for (int32_t j = rk + 1; j <= n; j++) {
            pre = min(pre, psa->height[j]);
            if (pre == 0) break;
            int32_t idx = psa->sa[j];
            if (idx < i) continue;
            if (idx - i <= pre) {
                mxLen = max(mxLen, idx - i);
            }
        }
        
        if (mxLen) {
            ans.eb(std::pii(i, i + mxLen), mxLen);
            i += 2 * mxLen;
        } else {
            i++;
        }
    }

    if (ans.empty()) {
        return;
    }

    sort(ans.begin(), ans.end());
    int fstLine = INF, lstLine = -INF;
    for (auto p : ans) {
        fstLine = min(fstLine, p.first.first);
        lstLine = max(lstLine, p.first.second);
    }


    std::vector<std::string> svninfo(1, "");
    if (is_svn) {
        const std::string &svnContent = getSVNInfo(filename);
        std::vector<std::string> svnlines(1, "");
        std::string strBuf;
        for (auto ch : svnContent) {
            if (ch == '\n') {
                svnlines.eb(strBuf);
                strBuf.clear();
            } else {
                strBuf.push_back(ch);
            }
        }
        if (!strBuf.empty()) {
            svnlines.eb(strBuf);
            strBuf.clear();
        }
        for (int i = 1; i <= n; i++) {
            int idx = lineIdx[i];
            const std::string &afterSVNProcessed = checkAndExtractSVNInfo(svnlines[idx], is_svn);
            svninfo.eb(afterSVNProcessed);
        }
    } else {
        svninfo = std::vector<std::string>(n + 1, "");
    }

    int32_t mxSVNInfoLen = 0;
    for (const std::string &str : svninfo) {
        mxSVNInfoLen = max(mxSVNInfoLen, str.size());
    }


    auto countDigit = [&](int64_t num) -> int64_t {
        int64_t ret = 0;
        while (num) {
            ret++;
            num /= 10;
        }
        return ret;
    };
    auto numToStringWithBlank = [&](int64_t num, int64_t width) -> std::string {
        std::string ret = "";
        while (num) {
            ret.push_back(num % 10 + '0');
            num /= 10;
        }
        reverse(ret.begin(), ret.end());
        assert(ret.size() <= width);
        while (ret.size() < width) {
            ret.push_back(' ');
        }

        return ret;
    };

    std::map<int32_t, int32_t> reIdx;
    std::map<int32_t, std::pii> mp;
    for (int32_t i = 0; i < ans.size(); i++) {
        std::pii p = ans[i].first;
        int32_t ansLen = ans[i].second;
        int32_t flag = 1;
        if (p.first + ansLen == p.second) {
            flag = 2;
        }
        for (int32_t j = 0; j < ansLen; j++) {
            mp[lineIdx[p.first + j]] = std::pii(lineIdx[p.second + j], flag);
            mp[lineIdx[p.second + j]] = std::pii(lineIdx[p.first + j], 10 + flag);
            reIdx[lineIdx[p.first + j]] = i + 1;
            reIdx[lineIdx[p.second + j]] = i + 1;
        }
    }

    std::cout << "文件: " << filename << " 发现可疑重复内容：" << endl;
    std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= 相关内容 =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    int32_t mxDigiLen = countDigit(lineIdx.back());
    for (int32_t i = max(1, fstLine - 30); i <= min(n, lstLine + 30); i++) {
        if (is_svn) std::cout << fitString(svninfo[i], mxSVNInfoLen) << "\t";
        std::pii nowp = mp[lineIdx[i]];
        if (nowp.second) {
            std::cout << (nowp.second < 10 ? "↓" : "↑") << numToStringWithBlank(reIdx[lineIdx[i]], 2) << " ";
        }
        std::cout << numToStringWithBlank(lineIdx[i], mxDigiLen);
        if (nowp.second) {
            std::cout << " - " << numToStringWithBlank(nowp.first, mxDigiLen) << ' ' << (nowp.second == 2 || nowp.second == 12 ? " * " : "   ");
        } else {
            std::cout << "       " << numToStringWithBlank(0, mxDigiLen) << ' ' << "   ";
        }
        std::cout << ":" << lines[i] << endl;
    }
    std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= Details =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=" << endl;

    std::cout << "===================================================================================================================================================================" << endl;
    for (int32_t i = 0; i < ans.size(); i++) {
        std::pii p = ans[i].first;
        int32_t ansLen = ans[i].second;
        if (p.first + ansLen == p.second) {
            std::cout << "*********************************************************************发现风险极高的重复内容***********************************************************************" << endl;
        }
        // std::cout << p.first << ' ' << p.second << endl;
        for (int32_t j = 0; j < ansLen; j++) {
            if (is_svn) std::cout << fitString(svninfo[p.first + j], mxSVNInfoLen) << "\t";
            std::cout << lineIdx[p.first + j] << " - " << lineIdx[p.second + j] << ": |" << lines[p.first + j] << endl;
        }
        // std::cout << "=-=-=-=-=-=-=-=-=-=" << endl;
        // for (int32_t j = p.second; j < p.second + ansLen; j++) {
        //     std::cout << lines[j] << "  -->  " << allHs[j].first << ' ' << allHs[j].second << endl;
        // }
        if (i != (int32_t)ans.size() - 1) {
            std::cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
        }
    }
    std::cout << "===================================================================================================================================================================" << endl;

    std::cout << endl << endl << endl;

    delete psa;
    delete pst;
}

int32_t processFileContent(const std::string& filename, const std::string& content, std::unique_ptr<FileType> fileType, const bool is_svn) {
    // std::cout << "内容大小: " << content.size() << " 字节" << endl;
    // std::string noChineseContent = removeChinese(content);
    const std::string &noChineseContent = content;
    // 在这里添加你的实际处理逻辑
    std::vector<std::pair<std::string, std::pair<int32_t, std::string> > > lines;
    std::string strBuf;

    int32_t lineCnt = 1;
    bool ignoreSwitch = false;
    for (auto ch : noChineseContent) {
        if (ch == '\n') {
            const std::string &svnInfo = "", &realContent = strBuf;
            std::pair<bool, bool> switchResult = fileType->CheckIgnoreSwitch(realContent, ignoreSwitch);
            ignoreSwitch = switchResult.first;
            if (fileType->CheckPush(realContent) && !ignoreSwitch && !switchResult.second) {
                lines.push_back(std::pair<std::string, std::pair<int32_t, std::string>> (realContent, std::pair<int32_t, std::string> (lineCnt, svnInfo)));
            }
            strBuf.clear();
            lineCnt++;
        } else {
            strBuf.push_back(ch);
        }
    }
    if (!strBuf.empty()) {
        const std::string &svnInfo = "", &realContent = strBuf;
        if (fileType->CheckPush(realContent)) {
            lines.push_back(std::pair<std::string, std::pair<int32_t, std::string>> (realContent, std::pair<int32_t, std::string> (lineCnt, svnInfo)));
        }
        strBuf.clear();
    }

    std::vector<std::pair<std::string, std::pair<int32_t, std::string> > > blkLinesBuf;
    for (auto psi : lines) {
        const std::string &str = psi.first;
        if (fileType->CheckSplitLine(str)) {
            if (blkLinesBuf.size() > 0) {
                processBlockContent(filename, blkLinesBuf, is_svn);
            }
            blkLinesBuf.clear();
        }

        blkLinesBuf.push_back(psi);
    }

    if (blkLinesBuf.size() > 0) {
        processBlockContent(filename, blkLinesBuf, is_svn);
    }

    return lineCnt;
}

// 读取文件内容
std::string readFileContent(const fs::path& filepath, const bool is_svn) {
    std::cerr << "处理文件：" << filepath << endl;
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filepath.string());
    }

    std::string content;
    // 获取文件大小
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // 读取内容
    content.resize(fileSize);
    file.read(&content[0], fileSize);

    // 统一换行符为 \n
    normalizeLineEndings(content);

    return content;
}

// 递归遍历目录
std::pair<int32_t, std::pii> processDirectory(const fs::path& directory, const bool is_svn) {
    std::pair<int32_t, std::pii> record;

    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.is_directory()) {
                // 如果是目录，递归处理
                std::pair<int32_t, std::pii> nxRecord = processDirectory(entry.path(), is_svn);
                record.first += nxRecord.first;
                record.second.first += nxRecord.second.first;
                record.second.second += nxRecord.second.second;

            } else if (entry.is_regular_file()) {
                const std::string &ext = entry.path().extension().string();
                std::unique_ptr<FileType> fileType = FileType::GetFileType(ext);
                if (fileType) {
                    // 如果是普通文件，读取并处理
                    try {
                        const std::string &content = readFileContent(entry.path(), is_svn);
                        record.second.first += processFileContent(entry.path().string(), content, std::move(fileType), is_svn);
                        record.second.second += content.size();
                        record.first++;
                    } catch (const std::exception& e) {
                        std::cerr << "处理文件出错: " << entry.path() << " - " << e.what() << endl;
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "遍历目录出错: " << directory << " - " << e.what() << endl;
    }

    return record;
}

int32_t main(int32_t argc, char* argv[]) {
    
#if defined(_WIN32)
    system("chcp 65001 > nul");
    std::cout << "== 检测到 Windows 平台 ==" << endl << endl;
#elif defined(__linux__)
    std::cout << "== 检测到 Linux 平台 ==" << endl << endl;
#endif

	std::ios::sync_with_stdio(false); std::cin.tie(nullptr); std::cout.tie(nullptr);

    if (!(argc == 2 || (argc == 3 && strcmp(argv[2], "--svn") == 0))) {
        std::cerr << "用法: " << argv[0] << " <目录路径> [--svn]" << endl;
        std::cerr << "将会遍历目录下所有 .lua / .cs 文件，输出结果在 output.txt " << endl;
        std::cerr << "如果带上 --svn 参数，则会同时引入提交人信息，但要求目录是svn目录" << endl;
        return 1;
    }

    freopen("output.txt", "w", stdout);

    fs::path directory(argv[1]);
    bool is_svn = (argc == 3 && strcmp(argv[2], "--svn") == 0);

    if (!fs::exists(directory)) {
        std::cerr << "错误: 目录不存在 - " << directory << endl;
        return 1;
    }

    if (!fs::is_directory(directory)) {
        std::cerr << "错误: 路径不是目录 - " << directory << endl;
        return 1;
    }

    std::cout << "开始处理目录: " << directory << endl;
    double st = clock();
    std::pair<int32_t, std::pii> record = processDirectory(directory, is_svn);
    double ed = clock();
    std::cout << "处理完成!" << endl;
    std::cout << "共处理文件: " << record.first << " 个." << endl;
    std::cout << "共处理代码行数: " << record.second.first << " 行." << endl;
    std::cout << "共处理字符数: " << record.second.second << " 个." << endl;
    std::cout << "耗时: " << (ed - st) / CLOCKS_PER_SEC << " 秒." << endl;

    return 0;
}
