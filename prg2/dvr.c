#include <stdio.h>

struct router {
    int dist[20];
    int next_hop[20];
} rt[20];

int main(){
    int n, cost[20][20], update;
    printf("Enter no of nodes : ");
    scanf("%d", &n);
    for (int i=0; i<n; i++){
        for (int j = 0; j<n; j++){
            scanf("%d", &cost[i][j]);
            if (i==j) cost[i][j] = 0;
            rt[i].dist[j] = cost[i][j];
            rt[i].next_hop[j] = j;
        }
    }
    
    do {
        update = 0;
        for (int i=0; i<n; i++){
            for (int j = 0; j<n; j++){
                for (int k = 0; k<n; k++){
                    if (rt[i].dist[j] > cost[i][k] + rt[k].dist[j]){
                        rt[i].dist[j] = cost[i][k] + rt[k].dist[j];
                        rt[i].next_hop[j] = k;
                        update++;
                    }
                }
            }
        }
    } while (update != 0);

    for(int i=0; i<n; i++){
        printf("Routing table for %d\n", i);
        printf("Dest\tNext Hop\tDist\n");
        for (int j = 0; j<n; j++){
            if (i != j)
                printf("%d\t%d\t\t%d\n",j,rt[i].next_hop[j], rt[i].dist[j]);
        }
        printf("\n");
    }
    return 0;
}