public class Board{
  
  private String[][] b;
  
  public Board(){
    b = new String[6][7];
    for(int i = 0; i<b.length; i++){
      for(int j = 0; j<b[0].length; j++){
        b[i][j] = "-";
      }
    }
  }
  
  public Board(String[][] s){
    b=s;
  }
  
  public void undo(int col){
    for(int i = 0; i<b.length; i++){
      if(!(b[i][col]=="-")){
        b[i][col]="-";
        return;
      }
    }
  }
  
  public String toString(){
    String s = "";
    for(int i = 0; i<b.length; i++){
      for(int j = 0; j<b[0].length; j++){
        s+=b[i][j]+"\t";
      }
      s+="\n";
    }
    return s;
  }
  
  public String[][] getArr(){
    return b;
  }
  
  public void move(int col, Player p){
    for(int i = b.length-1; i>=0; i--){
      if(b[i][col].equals("-")){
        b[i][col] = p.getSymbol();
        return;
      }
    }
  }
  
  public String winner(){
    for(int j = 0; j<b.length; j++){
      for(int i = 0; i<=b[0].length-4; i++){
        if(!b[j][i].equals("-")&&b[j][i].equals(b[j][i+1])&&b[j][i+2].equals(b[j][i+3])&&b[j][i].equals(b[j][i+3])){
          return b[j][i];
        }
      }
    }
    for(int i = b.length-1; i>=b.length-3; i--){
      for(int j = 0; j<b[0].length; j++){
        if(!b[i][j].equals("-")&&b[i][j].equals(b[i-1][j])&&b[i-2][j].equals(b[i-3][j])&&b[i][j].equals(b[i-3][j])){
          return b[i][j];
        }
      }
    }
    for(int i=b.length-1;i>=0;i--){
      for(int j=0; j<b[i].length;j++){
        if(i-4>=0 && j+4<b[i].length){
          if(!b[i][j].equals("-")&&b[i][j].equals(b[i-1][j+1])&&b[i-2][j+2].equals(b[i-3][j+3])&&b[i-2][j+2].equals(b[i][j])){
            return b[i][j];
          }
        }
      }
    }
    for(int i=b.length-1;i>=0;i--){
      for(int j=0; j<b[i].length;j++){
        if(i-4>=0 && j-4>=0){
          if(!b[i][j].equals("-")&&b[i][j].equals(b[i-1][j-1])&&b[i-2][j-2].equals(b[i-3][j-3])&&b[i-3][j-3].equals(b[i][j])){
            return b[i][j];
          }
        }
      }
    }
    return "-";
    }
  }
