
/* Revised:  2000 February 15, JAB */ 

typedef struct Initialize_Crate { /* Corrman to crate boss */ 
  uint32_t  msgCode; /* INITIALIZE_CRATE */ 
  uint32_t  taskID; 
  float     sysTempo; 
  uint32_t  bocfPeriod; 
  uint32_t  headerLen;
  uint32_t  links[2]; /* Index is crate */ 
} Initialize_Crate; 

typedef struct Crate_Status { /* Crate boss to corrman */ 
  uint32_t  msgCode; /* CRATE_STATUS */ 
  uint32_t  taskID; 
  uint32_t  linkStatus[64]; /* 0--31 crate 0; 32--63 crate 1 */ 
  /* More later */ 
} Crate_Status;  

typedef struct Task_Status { /* Crate boss to corrman */ 
  /* Response to Configure_Correlator */ 
  uint32_t  msgCode; /* TASK_STATUS */ 
  uint32_t  taskID; 
  uint32_t  status[2]; /* Index is crate */ 
      /* 0 is OK, any 1 bit is some errror */ 
} Task_Status; 

