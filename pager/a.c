#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// Function to check if a page exists in frame
bool isPagePresent(int page, int* frames, int frameCount) {
    for (int i = 0; i < frameCount; i++) {
        if (frames[i] == page)
            return true;
    }
    return false;
}

// Function to find the position of the first occurrence of a page in future
int findOptimalPosition(int* pages, int pageCount, int* frames, int frameCount, int currentPos) {
    int farthest = -1, replaceIndex = 0;
    
    for (int i = 0; i < frameCount; i++) {
        int j;
        for (j = currentPos + 1; j < pageCount; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    replaceIndex = i;
                }
                break;
            }
        }
        if (j == pageCount)
            return i;
    }
    return (farthest == -1) ? 0 : replaceIndex;
}

// FIFO Page Replacement Algorithm
void fifo(int* pages, int pageCount, int frameCount) {
    int* frames = (int*)calloc(frameCount, sizeof(int));
    int pageFaults = 0;
    int currentIndex = 0;
    
    printf("\nFIFO Page Replacement:\n");
    for (int i = 0; i < pageCount; i++) {
        printf("\nReference to page %d: ", pages[i]);
        
        if (!isPagePresent(pages[i], frames, frameCount)) {
            frames[currentIndex] = pages[i];
            currentIndex = (currentIndex + 1) % frameCount;
            pageFaults++;
            
            printf("Page Fault! Frames: ");
        } else {
            printf("No Page Fault. Frames: ");
        }
        
        for (int j = 0; j < frameCount; j++) {
            if (frames[j] != 0)
                printf("%d ", frames[j]);
            else
                printf("- ");
        }
    }
    printf("\nTotal Page Faults (FIFO): %d\n", pageFaults);
    free(frames);
}

// LRU Page Replacement Algorithm
void lru(int* pages, int pageCount, int frameCount) {
    int* frames = (int*)calloc(frameCount, sizeof(int));
    int* lastUsed = (int*)calloc(frameCount, sizeof(int));
    int pageFaults = 0;
    
    printf("\nLRU Page Replacement:\n");
    for (int i = 0; i < pageCount; i++) {
        printf("\nReference to page %d: ", pages[i]);
        
        if (!isPagePresent(pages[i], frames, frameCount)) {
            int replaceIndex = 0;
            int leastUsed = INT_MAX;
            
            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == 0) {
                    replaceIndex = j;
                    break;
                }
                if (lastUsed[j] < leastUsed) {
                    leastUsed = lastUsed[j];
                    replaceIndex = j;
                }
            }
            
            frames[replaceIndex] = pages[i];
            lastUsed[replaceIndex] = i;
            pageFaults++;
            
            printf("Page Fault! Frames: ");
        } else {
            printf("No Page Fault. Frames: ");
            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == pages[i]) {
                    lastUsed[j] = i;
                }
            }
        }
        
        for (int j = 0; j < frameCount; j++) {
            if (frames[j] != 0)
                printf("%d ", frames[j]);
            else
                printf("- ");
        }
    }
    printf("\nTotal Page Faults (LRU): %d\n", pageFaults);
    free(frames);
    free(lastUsed);
}

// Optimal Page Replacement Algorithm
void optimal(int* pages, int pageCount, int frameCount) {
    int* frames = (int*)calloc(frameCount, sizeof(int));
    int pageFaults = 0;
    
    printf("\nOptimal Page Replacement:\n");
    for (int i = 0; i < pageCount; i++) {
        printf("\nReference to page %d: ", pages[i]);
        
        if (!isPagePresent(pages[i], frames, frameCount)) {
            int replaceIndex;
            
            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == 0) {
                    replaceIndex = j;
                    break;
                }
            }
            
            if (frames[frameCount - 1] != 0) {
                replaceIndex = findOptimalPosition(pages, pageCount, frames, frameCount, i);
            }
            
            frames[replaceIndex] = pages[i];
            pageFaults++;
            
            printf("Page Fault! Frames: ");
        } else {
            printf("No Page Fault. Frames: ");
        }
        
        for (int j = 0; j < frameCount; j++) {
            if (frames[j] != 0)
                printf("%d ", frames[j]);
            else
                printf("- ");
        }
    }
    printf("\nTotal Page Faults (Optimal): %d\n", pageFaults);
    free(frames);
}

int main() {
    int pageCount, frameCount;
    
    printf("Enter the number of pages: ");
    scanf("%d", &pageCount);
    
    printf("Enter the number of frames: ");
    scanf("%d", &frameCount);
    
    int* pages = (int*)malloc(pageCount * sizeof(int));
    
    printf("Enter the page reference string: ");
    for (int i = 0; i < pageCount; i++) {
        scanf("%d", &pages[i]);
    }
    
    fifo(pages, pageCount, frameCount);
    lru(pages, pageCount, frameCount);
    optimal(pages, pageCount, frameCount);
    
    free(pages);
    return 0;
}
