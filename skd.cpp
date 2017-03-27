#include <iostream>
//#include<time.h>
//#include <algorithm>

using namespace std;

class puzzle{

    int su_in[9][9], su_out[9][9];


public:
   // void read(int);
    void setval(int val[9][9]);
    void display(int);
    int validate(int);
    int box_validate(int,int,int);
    int sum_validate();
    int conflict();
    int solve();
    void elem();
    void elem_box(int,int);
    void elem_row(int,int);
    void elem_col(int ,int);
    void assign();
    void assign_poss(int ,int);
    void guess(int);
    void best(int,int,int,int);
    void back_up(int);
    void backtrack(int);
    void restore(int);
    int check();
    void test();

};

 struct best{

        int row;
        int col;
    }best_cell;

class chance{

public:	int poss[9];
        int total_poss;

}cell[9][9];


class backup_all{
public:	int poss[9];
        int total_poss;
        int cell;
    }backup[100][9][9];

const double CLK_TCK = 1000.0;
clock_t st,et;

int main()
{
    st=clock();
     int skd[9][9]={
                     0,0,7   ,0,0,0    ,4,0,6,
                     8,0,0   ,4,0,0    ,1,7,0,
                     0,0,0   ,3,0,0    ,9,0,5,

                     0,0,0   ,7,0,5    ,0,0,8,
                     0,0,0   ,0,0,0    ,0,0,0,
                     4,0,0   ,2,0,8    ,0,0,0,

                     7,0,4   ,0,0,3    ,0,0,0,
                     0,5,2   ,0,0,1    ,0,0,9,
                     1,0,8   ,0,0,0    ,6,0,0

               };/*  
   int skd[9][9] ={
            0,6,0   ,0,0,2    ,0,0,0,
            9,0,0   ,5,4,6    ,0,8,0,
            0,0,0   ,3,0,0    ,0,0,5,

            7,0,0   ,0,0,0    ,0,3,6,
            0,0,3   ,0,1,0    ,5,0,0,
            5,8,0   ,0,0,0    ,0,0,2,

            3,0,0   ,0,0,4    ,0,0,0,
            0,2,0   ,1,8,9    ,0,0,7,
            0,0,0   ,7,0,0    ,0,0,0
           };*/ // This sudoku does not solve.
    int get_res;
   // float time;
    puzzle fun;

    fun.setval(skd);

   // fun.read();
   enum {input=1,output};
   fun.display(input);
   get_res=fun.validate(input);
   if(get_res==0)
       cout<<"valid input!\n";
       else if(get_res==1)
          cout<<"invalid input!\n";


   fun.solve();

   //if(validate(output))
       //cout<<"\tNOT SOLVED\n";
   //else  {
   cout<<"\t\tTHE SOLUTION IS:\n\n";
   fun.display(output);
   et=clock();
//	}
  // time=(et-st);
   //cout<<"the total time taken to solve is: "<<time/CLK_TCK<<" seconds\n";


}

void puzzle::setval(int val[][9])
{
    int i,j,k;

    for(i=0; i<9; i++)
       for(j=0; j<9; j++){
           su_in[i][j]=val[i][j];
           su_out[i][j]=val[i][j];
            }
    for(i=0;	i<9;	i++)						//inisilize all possiblilites of 1-9 in empty cells;//
      for(j=0;	j<9;	j++)
            {
         if(su_in[i][j]==0)	{
          for(k=0;	k<9;	k++)
             cell[i][j].poss[k]=k+1;
        }
         else cell[i][j].poss[su_in[i][j]-1]=su_in[i][j];
        }
}


void puzzle::display(int sudoku)   // function to display the SU-DO-CU
{
    int i,j;

        for(i=0;i<9; i++)
        {
         if(i%3==0)
          cout<<"\t\t-------------------------\n";
              cout<<"\t\t";
         for(j=0; j<9; j++)
          {
            if(j%3==0)
             cout<<"| ";
            if(sudoku==1)
             cout<<su_in[i][j]<<" ";
            else if(sudoku==2)
             cout<<su_out[i][j]<<" ";
            if(j==8)
             cout<<"|";
          }
            cout<<"\n";
            if(i==8)
            cout<<"\t\t-------------------------\n";
         }
}

int puzzle::validate(int sudoku) // function to validate the input SU-DO-CU
{
    int i,j,k;
//.................................row validation.
    for(i=0; i<9; i++)
      for(j=0; j<9; j++)
        for(k=0;k<9;k++)
        {
          if(sudoku==1 && k!=j && su_in[i][j]!=0)
           {
             if(su_in[i][j]==su_in[i][k])
             return 1;
           }
        else if(sudoku==2 && k!=j && su_out[i][j]!=0 )
           {
             if(su_out[i][j]==su_out[i][k])
             return 1;
           }

        }
//..................................colume validation.
    for(i=0;  i<9;  i++)
     for(j=0;  j<9;  j++)
       for(k=0;  k<9;  k++)
        {
         if(sudoku==1 && k!=j && su_in[j][i]!=0)
        {
          if(su_in[j][i]==su_in[k][i])
          return 1;
        }
         else if(sudoku==2 && k!=i && su_out[j][i]!=0)
        {
          if(su_out[j][i]==su_out[j][k])
          return 1;
        }
        }
    //.............................. each box validating.
    for(i=0;  i<=6;  i=i+3)
      for(j=0; j<=6; j=j+3)
       {
        if(box_validate(i,j,sudoku)==1)
        return 1;
       }

       return 0;
}


int puzzle::box_validate(int i,int j,int sudoku)
{
    int k=j,n=i;
    int temp_i=i,temp_j=j,temp_k=k, temp_n=n;

    for(i=temp_i;  i<temp_i+3;  i++)
      for(j=temp_j;  j<temp_j+3;  j++)
       for(k=temp_k;  k<temp_k+3;  k++)
        {
          if(sudoku==1 && k!=j && su_in[i][j]!=0)
            {
            if(su_in[i][j]==su_in[i][k])
            return 1;

            for(n=temp_n;  n<temp_n+3;  n++)
            {
              if(sudoku==1 &&  su_in[i][j]!=0){
              if(k!=j || n!=i)
                if(su_in[i][j]==su_in[n][k])
                      return 1;
                    }
            }
            }

            if(sudoku==2 && k!=j && su_out[i][j]!=0)
              {
               if(su_out[i][j]==su_out[i][k])
               return 1;
               for(n=temp_n;  n<temp_n+3;  n++)
                 {
                if(sudoku==2 && su_out[i][j]!=0 ){
                if(k!=j || n!=i)
                    if(su_out[i][j]==su_out[n][k])
                        return 1;}
                 }
              }
            }
    return 0;

}

int puzzle::sum_validate()
{
    int i,j,count=0;

    for(i=0; i<9; i++)
      for(j=0; j<9; j++)
        count=count+su_out[i][j];

    if(count!=405) return 1;
    else return 0;

}

int puzzle::conflict()
{
    int i,j,count,x;
                     // 	   check confilict possibilities in the column..
    for(i=0; i<9; i++)
      for(j=0; j<9; j++)
       {
         count=0;
         if(su_out[i][j]==0)
         {
          for(x=0; x<9; x++) {
          if(cell[i][j].poss[x]!=0)
          count++;
                 }
          if(count==0)
          return 1;

          }

        }
     if(validate(2))
       return 1;
     return 0;

}


int puzzle::solve()
{
    int no_guess=0,count=0,i,j,x;
    //float time;
    elem();
    assign();
  //	et=clock();

        for(double x=0; ;  x++)
        {
            no_guess++;
            guess(no_guess);

            count=0;

             while(count<20)
             {
             elem();
             assign();
             count++;
             }


             while(conflict()!=0)
            {
                 if(no_guess==0)
                     break;

                backtrack(no_guess);
                 no_guess--;
    //			 cout<<"no of guess= "<<no_guess<<endl;

           }

                if(check())
                break;
                //cout<<"no_guess: "<<no_guess<<endl;
        //	et=clock();
        }

           for(i=0; i<9; i++)						// find the no. of possibilities in a cell.
          for(j=0; j<9; j++)
          {
            count=0;
            if(su_out[i][j]==0)
            {
                for(x=0; x<9; x++)
                 {
                    if(cell[i][j].poss[x]!=0)
                     count++;
                }
              cell[i][j].total_poss=count;
            }
            else cell[i][j].total_poss=1;
          }

                                    count=0;

    /*	 for(i=0;	conflicti<9; i++)
               for(j=0;	j<9;  j++)
                   {
                   cout<<"total poss is:" << cell[i][j].total_poss<<endl;
                   count=cell[i][j].total_poss+count;
                    }
               cout<<"total= "<< count<<endl;*/
     return 0;
}

void puzzle::elem()
{

int  i,j;

  for(i=0;	i<=6;	i+=3)
    for(j=0;	j<=6;	j+=3)  {
            elem_box(i,j);	   //eliminates box possibbility.
            elem_row(i,j);	   //eliminates row possibilites.
            elem_col(i,j);	   //eliminates colum possibilites.
                     }

}


void puzzle::assign()

{
    int  i,j;
    for(i=0;	i<=6;	i+=3)
                     for(j=0;	j<=6;	j+=3)
                         assign_poss(i,j);
}


void puzzle::elem_box(int i, int j)			//function to eliminate all numbers present in box from the possiblilty.
{
         int temp_k=i,  temp_m=j,k,m;
         int i_max=i+3,temp_j=j,temp_i=i;
         int j_max=j+3;

          for(i=temp_i; i<i_max; i++)
             for(j=temp_j; j<j_max; j++)
                 if(su_out[i][j]==0)
                 {
                     for(k=temp_k;	k<i_max;	k++)
                         for(m=temp_m;	m<j_max;	m++)
                             if(su_out[k][m]!=0)
                                 cell[i][j].poss[su_out[k][m]-1]=0;
                 }

}

void puzzle::elem_row(int i,int j)		 //function to eliminate all numbers present in the same row from possibility.

{
    int temp_i=i,temp_j=j;
    int i_max=i+3,j_max=j+3;
    int m;

    for(i=temp_i; i<i_max;	i++)
        for(j=temp_j; j<j_max; j++){
            if(su_out[i][j]==0)
                for(m=0; m<9;	m++)
                    if(su_out[i][m]!=0)
                        cell[i][j].poss[su_out[i][m]-1]=0;
        }

}

void puzzle::elem_col(int i, int	j) 	 //function to eliminate all numbers present in the same row from possibility.
{
    int temp_i=i,temp_j=j;
    int i_max=i+3,j_max=j+3,k;


    for(i=temp_i; i<i_max;	i++)
        for(j=temp_j; j<j_max; j++){
            if(su_out[i][j]==0)
                for(k=0; k<9;	k++)
                    if(su_out[k][j]!=0)
                        cell[i][j].poss[su_out[k][j]-1]=0;
        }

}

void puzzle::assign_poss(int i, int j)
{

     int k,temp;
     int val=0,x,m;
     int i_max=6,temp_j=j,temp_i=i;
     int j_max=3;
     int count;

    //	 if only one possiblilty in a box. =>assign it to cell which contain that.

     for(i=temp_i; i<i_max; i++)
             for(j=temp_j; j<j_max; j++)
                 if(su_out[i][j]==0)
                 {

                    for(x=0; x<9; x++)
                    {
                        count=0;
                        if(cell[i][j].poss[x]==0)
                         continue;
                        else {
                            val=cell[i][j].poss[x];
                            for(k=temp_i;	k<i_max;	k++)
                             for(m=temp_j;	m<j_max;	m++)
                              {
                                if(su_out[k][m]==0)
                                  {
                                    if(cell[k][m].poss[x]==val)
                                       count++;
                                  }

                             }

                         if(count==1)
                                 su_out[i][j]=val;
                        }

                    }

                 }

                                        // if one possibily for a cell=> assign that to su_out.
     for(i=0;	i<9;	i++)
        for(j=0;	j<9;	j++) {
            count=0;
            for(k=0; k<9; k++)
             if(cell[i][j].poss[k]!=0)
               count++,temp=cell[i][j].poss[k];

              if(count==1)
                 su_out[i][j]=temp;
        }



}

 int puzzle::check()
{

    for(unsigned i=0; i<9; i++)
        for(unsigned j=0; j<9; j++)
            {
                if(su_out[i][j]==0)
                return 0;
            }

        if(validate(2))
            return 0;

    return 1;

}

 void puzzle::guess(int no_guess)
 {
    int i, j, x, count=0,flag=0;


      for(i=0; i<9; i++)						// find the no. of possibilities in a cell.
          for(j=0; j<9; j++)
          {
            count=0;
            if(su_out[i][j]==0)
            {
                for(x=0; x<9; x++)
                 {
                    if(cell[i][j].poss[x]!=0)

                                    count++;
                }
              cell[i][j].total_poss=count;
            }
            else cell[i][j].total_poss=1;
          }



    for(x=2; x<9; x++)	  // find the best cell with lest possibility.
    {
      for(i=0; i<9; i++)
         {
             for(j=0; j<9; j++)
               if(cell[i][j].total_poss==x)
               {
                 best_cell.row=i;
                 best_cell.col=j;
                 flag=1;
                 break;
              }
          if(flag==1)
            break;
      }
     if(flag==1)
      break;
    }


   // for(i=0; i<9; i+=3)
     //   for(j=0; j<9; j+=3)
       //     best(i,j,best_cell.row, best_cell.col);


//	cout<<"best cell is: su_out "	<< best_cell.row<<" " <<best_cell.col<<endl;
    //cout<<"with total poss of: "	<<cell[best_cell.row][best_cell.col].total_poss<<endl;
    //cout<<"no of guess: "<<no_guess<<endl;

    back_up(no_guess);

    for(x=0; x<9; x++)         //perform the guess..
     if(cell[best_cell.row][best_cell.col].poss[x] !=0)
     {
         su_out[best_cell.row][best_cell.col]=cell[best_cell.row][best_cell.col].poss[x];
         break;
     }


 }

/* void best (int i, int j,int best_r, int best_c)
 {
    // for(i=0; i<)



 }*/

 void puzzle::back_up(int no_guess)
 {
    int i,j,x;

    // back up the initial sudoku...

    for(i=0; i<9; i++)			// cell back up...
          for(j=0; j<9; j++) {
              backup[no_guess][i][j].cell=su_out[i][j];
              }

     for(i=0; i<9; i++)			// poss back up...
          for(j=0; j<9; j++)
             for(x=0; x<9; x++)
                  backup[no_guess][i][j].poss[x]=cell[i][j].poss[x];



     for(i=0; i<9; i++)			// total poss back up...
          for(j=0; j<9; j++)
             for(x=0; x<9; x++)
                 backup[no_guess][i][j].total_poss=cell[i][j].total_poss;


 }

 void puzzle::restore(int no_guess)
 {
        int i,j,x;

    // restore the last backup sudoku...

    for(i=0; i<9; i++)			// cell restore...
          for(j=0; j<9; j++) {
              su_out[i][j]=backup[no_guess][i][j].cell;
              }

     for(i=0; i<9; i++)			// poss restore...
          for(j=0; j<9; j++)
             for(x=0; x<9; x++)
                  cell[i][j].poss[x]=backup[no_guess][i][j].poss[x];



     for(i=0; i<9; i++)			// total poss restore...
          for(j=0; j<9; j++)
             for(x=0; x<9; x++)
                 cell[i][j].total_poss=backup[no_guess][i][j].total_poss;


 }

 void puzzle::backtrack(int no_guess)
 {
     int x;
                                      // assign the initial stage befor the guess..
        restore(no_guess);

                                      // eliminate the guessed num from poss..
     for(x=0; x<9; x++)
     if(cell[best_cell.row][best_cell.col].poss[x] !=0)
     {
        cell[best_cell.row][best_cell.col].poss[x]=0;

        //elem();

        //assign();
        break;
     }

 }






