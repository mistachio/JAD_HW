package testCAD;

public class Message
{
	static Draw draw= new Draw();
	static Draft draft = new Draft();
    public  Listener listener;  //��Ӧ�û�
    public static String state="idle";
	public Message(Draft draft) {
		this.draft = draft;
	}
	public static void updateView()   //���½���
    {
		draft.printAllShape(draw.shapes);
    }
}

