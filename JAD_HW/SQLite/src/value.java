
public abstract class value { //抽象类value
	public String val;
	value(String S){
		this.val = S;
	}
}

class Int extends value{  //int整型value

	Int(String S) {
		super(S);
	} 
}

class Real extends value{  //real实数型value

	Real(String S) {
		super(S);
	} 
}

class Char extends value{  // char 字符串型value

	Char(String S) {
		super(S);
	} 
}