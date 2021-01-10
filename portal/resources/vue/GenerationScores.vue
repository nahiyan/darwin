<template>
  <div>
    <Navigation
      :extension="generation.session.extension"
      :session="generation.session"
      :generation="generation"
    ></Navigation>
    <h1>Scores of Generation {{ generation.id }}</h1>
    <ChartComponent />
  </div>
</template>

<script>
import ChartComponent from "./Chart";
import ApexCharts from "apexcharts";
import Navigation from "../vue/Navigation";

export default {
  data() {
    return {
      generation: { session: { extension: null } },
      scores: [],
    };
  },
  mounted() {
    window.axios
      .get("/api/generation_scores/" + this.$route.params.id)
      .then((response) => {
        const data = response.data;
        this.scores = data.scores;
        this.generation = data.generation;

        var options = {
          series: [
            {
              name: "Score",
              data: Array.from(this.scores, (number, _) => number.toFixed(2)),
            },
          ],
          chart: {
            type: "bar",
            animations: {
              enabled: false,
              easing: "easeinout",
              speed: 500,
              animateGradually: {
                enabled: true,
                delay: 150,
              },
              dynamicAnimation: {
                enabled: true,
                speed: 350,
              },
            },
          },
          plotOptions: {
            bar: {
              horizontal: true,
              dataLabels: {
                position: "top", // top, center, bottom
              },
            },
          },
          dataLabels: {
            enabled: false,
            formatter: function (val) {
              return val;
            },
            offsetY: -20,
            style: {
              fontSize: "12px",
              colors: ["#304758"],
            },
          },

          xaxis: {
            title: {
              text: "Scores",
            },
            categories: Array.from({ length: this.scores.length }, (_, i) =>
              (i + 1).toString()
            ),
            position: "bottom",
            axisBorder: {
              show: false,
            },
            axisTicks: {
              show: false,
            },
            crosshairs: {
              fill: {
                type: "gradient",
                gradient: {
                  colorFrom: "#D8E3F0",
                  colorTo: "#BED1E6",
                  stops: [0, 100],
                  opacityFrom: 0.4,
                  opacityTo: 0.5,
                },
              },
            },
            tooltip: {
              enabled: true,
            },
          },
          yaxis: {
            axisBorder: {
              show: false,
            },
            axisTicks: {
              show: false,
            },
            labels: {
              show: true,
            },
            title: {
              text: "Members' Ranks",
            },
          },
          title: {
            floating: true,
            offsetY: 330,
            align: "center",
            style: {
              color: "#444",
            },
          },
        };

        var chart = new ApexCharts(document.querySelector("#chart"), options);

        chart.render();
      });
  },
  components: {
    ChartComponent,
    Navigation,
  },
};
</script>