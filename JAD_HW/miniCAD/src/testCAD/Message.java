package testCAD;

public class Message
{
	static Draw draw= new Draw();
	static Draft draft = new Draft();
    public  Listener listener;  //相应用户
    public static String state="idle";
	public Message(Draft draft) {
		this.draft = draft;
	}
	public static void updateView()   //更新界面
    {
		draft.printAllShape(draw.shapes);
    }
}

