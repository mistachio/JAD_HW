package testCAD;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.GridLayout;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;

import testCAD.Message;


public class CAD_main extends JFrame {
	int y_size = 800;
	int x_size = 1000;
	JFrame frame;
	int num_of_shapes = 0;
	static Draft paper = new Draft();

	public CAD_main(){
		this.setTitle("mini CAD");
		this.setSize(840, 560);
        this.getContentPane().setBackground(Color.white);
		
		JButton button_1 = new JButton("circle");
		JButton button_2 = new JButton("square");
		JButton button_3 = new JButton("line");
		JButton button_4 = new JButton("select");
		JButton button_5 = new JButton("delete");
		
		
		JPanel panel_buttons = new JPanel(new GridLayout(5,1));
		this.add(BorderLayout.EAST, panel_buttons);
		this.add(BorderLayout.CENTER, paper);
		panel_buttons.add(button_1);
		panel_buttons.add(button_2);
		panel_buttons.add(button_3);
		panel_buttons.add(button_4);
		panel_buttons.add(button_5); 
		Listener listener = new Listener();
		button_1.addActionListener(listener);
		button_2.addActionListener(listener);
		button_3.addActionListener(listener);
		button_4.addActionListener(listener);
		button_5.addActionListener(listener);
		
		
        JMenuBar menuBar=new JMenuBar();
        JMenu menu1=new JMenu("File");
        JMenuItem menuItem1=new JMenuItem("Open");
        JMenuItem menuItem2=new JMenuItem("Save");
        

        JMenu menu2=new JMenu("Set Color");
        JMenuItem menuItem3=new JMenuItem("Gray");
        JMenuItem menuItem4=new JMenuItem("White");
        JMenuItem menuItem5=new JMenuItem("Red");
        JMenuItem menuItem6=new JMenuItem("Blue");
        JMenuItem menuItem7=new JMenuItem("Yellow");
        JMenuItem menuItem8=new JMenuItem("Green");

        menu2.add(menuItem3);
        menu2.add(menuItem4);
        menu2.add(menuItem5);
        menu2.add(menuItem6);
        menu2.add(menuItem7);
        menu2.add(menuItem8);


        menuItem1.addActionListener(listener);
        menuItem2.addActionListener(listener);
        menuItem3.addActionListener(listener);
        menuItem4.addActionListener(listener);
        menuItem5.addActionListener(listener);
        menuItem6.addActionListener(listener);
        menuItem7.addActionListener(listener);
        menuItem8.addActionListener(listener);
        menu1.add(menuItem1);
        menu1.add(menuItem2);
        
        menuBar.add(menu1);
        menuBar.add(menu2);

        this.setJMenuBar(menuBar);
		this.setVisible(true);
		setLocationRelativeTo (null);
        
	}
	


	public static void main(String[] args) {
		CAD_main frame = new CAD_main();
        Message controller = new Message(paper);
	}
}
