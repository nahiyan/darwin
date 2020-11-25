<template>
  <div>
    <Navigation
      :extension="session.extension"
      :session_scores="session"
    ></Navigation>
    <h1>Scores of Session {{ session.id }}</h1>
    <ChartComponent />
  </div>
</template>

<script>
import ChartComponent from "./Chart";
import Chart from "chart.js";
import Navigation from "../vue/Navigation";

export default {
  data() {
    return {
      session: {},
      scores: [],
    };
  },
  mounted() {
    window.axios
      .get("/api/session_scores/" + this.$route.params.id)
      .then((response) => {
        const data = response.data;
        this.scores = data.scores;
        this.session = data.session;

        var chart = new Chart("chart", {
          type: "line",
          data: {
            labels: Array.from({ length: this.scores.length }, (_, i) => i + 1),
            datasets: [
              {
                label: "Scores",
                // backgroundColor: "rgb(255, 99, 132)",
                borderColor: "rgb(255, 99, 132)",
                data: this.scores,
              },
            ],
          },
          options: { animation: { duration: 0 } },
        });
      });
  },
  components: {
    ChartComponent,
    Navigation,
  },
};
</script>