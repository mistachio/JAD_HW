
public abstract class value { //������value
	public String val;
	value(String S){
		this.val = S;
	}
}

class Int extends value{  //int����value

	Int(String S) {
		super(S);
	} 
}

class Real extends value{  //realʵ����value

	Real(String S) {
		super(S);
	} 
}

class Char extends value{  // char �ַ�����value

	Char(String S) {
		super(S);
	} 
}