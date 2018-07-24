package group12.sparc_moblieapp;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import butterknife.BindView;
import butterknife.ButterKnife;


public class ScheduleActivity  extends AppCompatActivity {


    @BindView(R.id.input_day) EditText in_Day;
    @BindView(R.id.input_Time) EditText in_Time;
    @BindView(R.id.btn_setSch) Button sch_set;



    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_schedule);
        ButterKnife.bind(this);


        sch_set.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {

                String Day = in_Day.getText().toString();
                String Time =in_Time.getText().toString(); //Grabs the input string

                Intent intent = new Intent(getApplicationContext(), MainActivity.class);
                intent.putExtra("schedule", new Schedule(Day,Time));
               startActivity(intent);
            }
        });

    }



}
