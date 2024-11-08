
typedef struct Initialize_Crate { /* Corrman to crate boss */ 
  uint32_t  msgCode; /* INITIALIZE_CRATE */ 
  uint32_t  taskID; 
  float     sysTempo; 
  uint32_t  bocfPeriod; 
} Initialize_Crate; 

typedef struct Crate_Status { /* Crate boss to corrman */ 
  uint32_t  msgCode; /* CRATE_STATUS */ 
  uint32_t  taskID; 
  uint32_t  status; 
  /* More later */ 
} Crate_Status;  

